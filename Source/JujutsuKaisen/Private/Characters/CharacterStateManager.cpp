// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/CharacterStateManager.h"
#include "Characters/JujutsuKaisenCharacter.h"

UCharacterStateManager::UCharacterStateManager()
{
	CurrentState = ECharacterState::Locomotion;
	CurrentHitSubState = EHitSubState::Custom;
	OwnerCharacter = nullptr;
}

void UCharacterStateManager::SetOwnerCharacter(AJujutsuKaisenCharacter* Character)
{
	OwnerCharacter = Character;
}

bool UCharacterStateManager::SetState(ECharacterState NewState)
{

	if (!CanTransitionTo(NewState))
	{
		return false;
	}

	ForceState(NewState);

	
	return true;
}

bool UCharacterStateManager::SetHitSubState(EHitSubState NewSubState, bool bIsHitFront)
{
	SetState(ECharacterState::Hit);
	if (CurrentState == ECharacterState::Hit)
	{
		CurrentHitSubState = NewSubState;
		IsHitFront = bIsHitFront;
		return true;
	}
	return false;
}

bool UCharacterStateManager::IsInState(ECharacterState State) const
{
	return CurrentState == State;
}

bool UCharacterStateManager::CanTransitionTo(ECharacterState NewState) const
{
	// 기본적으로 우선순위가 높은 상태로만 전환 가능
	return static_cast<uint8>(NewState) >= static_cast<uint8>(CurrentState);
}

void UCharacterStateManager::ForceState(ECharacterState NewState)
{
	const ECharacterState PreviousState = CurrentState;
	CurrentState = NewState;
	UpdateGravityForState(NewState);

	auto StateToString = [](ECharacterState State) -> const TCHAR*
	{
		switch (State)
		{
		case ECharacterState::Locomotion: return TEXT("Locomotion");
		case ECharacterState::Falling:    return TEXT("Falling");
		case ECharacterState::Skill:      return TEXT("Skill");
		case ECharacterState::Hit:        return TEXT("Hit");
		case ECharacterState::Dead:       return TEXT("Dead");
		default:                          return TEXT("Unknown");
		}
	};

	const FString Message = FString::Printf(TEXT("State change: %s -> %s"),
		StateToString(PreviousState),
		StateToString(NewState));

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, Message);
	}
}

void UCharacterStateManager::ResetSubStates(ECharacterState NewState)
{
	switch (NewState)
	{
	case ECharacterState::Hit:
		CurrentHitSubState = EHitSubState::Custom;
		IsHitFront = true;
		break;
	case ECharacterState::Locomotion:
	case ECharacterState::Falling:
	case ECharacterState::Dead:
	case ECharacterState::Skill:
		// 하위 상태가 없는 상태들
		break;
	}
}

void UCharacterStateManager::UpdateGravityForState(ECharacterState NewState)
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateGravityForState: OwnerCharacter is NULL!"));
		return;
	}

	// 스킬 상태일 때만 중력 끄기
	if (NewState == ECharacterState::Skill)
	{
		OwnerCharacter->SetGravityEnabled(false);
	}
	else
	{
		// 다른 모든 상태에서는 중력 켜기
		OwnerCharacter->SetGravityEnabled(true);
	}
}