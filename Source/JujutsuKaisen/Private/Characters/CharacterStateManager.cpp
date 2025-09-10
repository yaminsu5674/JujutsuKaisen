// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/CharacterStateManager.h"

UCharacterStateManager::UCharacterStateManager()
{
	CurrentState = ECharacterState::Locomotion;
	CurrentHitSubState = EHitSubState::CustomHit;
}

bool UCharacterStateManager::SetState(ECharacterState NewState)
{

	if (!CanTransitionTo(NewState))
	{
		return false;
	}

	CurrentState = NewState;
	ResetSubStates(NewState);
	
	// 현재 상태 디버그 메시지 출력
	if (GEngine)
	{
		FString StateName;
		switch (CurrentState)
		{
		case ECharacterState::Dead:
			StateName = TEXT("Dead");
			break;
		case ECharacterState::Hit:
			StateName = TEXT("Hit");
			break;
		case ECharacterState::Skill:
			StateName = TEXT("Skill");
			break;
		case ECharacterState::Falling:
			StateName = TEXT("Falling");
			break;
		case ECharacterState::Locomotion:
			StateName = TEXT("Locomotion");
			break;
		default:
			StateName = TEXT("Unknown");
			break;
		}
		
		GEngine->AddOnScreenDebugMessage(
			-1, // Key (-1은 항상 표시)
			3.0f, // 화면에 표시되는 시간 (초)
			FColor::Yellow, // 색상
			FString::Printf(TEXT("현재 상태: %s"), *StateName) // 메시지
		);
	}
	
	return true;
}

bool UCharacterStateManager::SetHitSubState(EHitSubState NewSubState)
{
	if (CurrentState == ECharacterState::Hit)
	{
		CurrentHitSubState = NewSubState;
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
	CurrentState = NewState;
	ResetSubStates(NewState);
	
	// 현재 상태 디버그 메시지 출력 (ForceState용)
	if (GEngine)
	{
		FString StateName;
		switch (CurrentState)
		{
		case ECharacterState::Dead:
			StateName = TEXT("Dead");
			break;
		case ECharacterState::Hit:
			StateName = TEXT("Hit");
			break;
		case ECharacterState::Skill:
			StateName = TEXT("Skill");
			break;
		case ECharacterState::Falling:
			StateName = TEXT("Falling");
			break;
		case ECharacterState::Locomotion:
			StateName = TEXT("Locomotion");
			break;
		default:
			StateName = TEXT("Unknown");
			break;
		}
		
		GEngine->AddOnScreenDebugMessage(
			-1, // Key (-1은 항상 표시)
			3.0f, // 화면에 표시되는 시간 (초)
			FColor::Cyan, // 색상 (ForceState는 다른 색상으로 구분)
			FString::Printf(TEXT("ForceState: %s"), *StateName) // 메시지
		);
	}
}

void UCharacterStateManager::ResetSubStates(ECharacterState NewState)
{
	switch (NewState)
	{
	case ECharacterState::Hit:
		CurrentHitSubState = EHitSubState::CustomHit;
		break;
	case ECharacterState::Locomotion:
	case ECharacterState::Falling:
	case ECharacterState::Dead:
	case ECharacterState::Skill:
		// 하위 상태가 없는 상태들
		break;
	}
}