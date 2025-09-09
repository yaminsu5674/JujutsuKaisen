// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/CharacterStateManager.h"

UCharacterStateManager::UCharacterStateManager()
{
	CurrentState = ECharacterState::Locomotion;
	CurrentHitSubState = EHitSubState::None;
	CurrentGuardSubState = EGuardSubState::None;
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

bool UCharacterStateManager::SetGuardSubState(EGuardSubState NewSubState)
{
	if (CurrentState == ECharacterState::Guard)
	{
		CurrentGuardSubState = NewSubState;
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
	// 상태 전환 우선순위 체크
	return CheckStatePriority(NewState);
}

bool UCharacterStateManager::CanPerformAction() const
{
	// 액션 수행 가능한 상태인지 체크
	switch (CurrentState)
	{
	case ECharacterState::Locomotion:
	case ECharacterState::Land:
		return true;
	case ECharacterState::Skill:
		// 스킬 상태는 몽타주 재생 중이므로 액션 불가
		return false;
	case ECharacterState::Hit:
	case ECharacterState::Guard:
	case ECharacterState::Dead:
		return false;
	default:
		return false;
	}
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
		CurrentHitSubState = EHitSubState::None;
		break;
	case ECharacterState::Guard:
		CurrentGuardSubState = EGuardSubState::Guarding;
		break;
	case ECharacterState::Locomotion:
	case ECharacterState::Land:
	case ECharacterState::Dead:
		// 하위 상태가 없는 상태들
		break;
	}
}

bool UCharacterStateManager::CheckStatePriority(ECharacterState NewState) const
{
	// 우선순위: Dead > Hit > Guard > Skill > Locomotion > Land
	// 낮은 숫자가 높은 우선순위
	return static_cast<uint8>(NewState) >= static_cast<uint8>(CurrentState);
}
