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