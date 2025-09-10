// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/CharacterStateManager.h"

UCharacterStateManager::UCharacterStateManager()
{
	CurrentState = ECharacterState::Locomotion;
	CurrentHitSubState = EHitSubState::None;
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
	// 죽음 상태는 모든 상태를 덮어쓸 수 있음
	if (NewState == ECharacterState::Dead)
	{
		return true;
	}

	// 현재 상태가 죽음이면 다른 상태로 전환 불가
	if (CurrentState == ECharacterState::Dead)
	{
		return false;
	}

	// 피격 상태는 다른 모든 상태를 덮어쓸 수 있음 (죽음 제외)
	if (NewState == ECharacterState::Hit)
	{
		return true;
	}

	// 현재 상태가 피격이면 다른 상태로 전환 불가 (죽음 제외)
	if (CurrentState == ECharacterState::Hit)
	{
		return false;
	}

	// 스킬 상태는 로코모션, 낙하 상태를 덮어쓸 수 있음
	if (NewState == ECharacterState::Skill)
	{
		return CurrentState == ECharacterState::Locomotion || CurrentState == ECharacterState::Falling;
	}

	// 로코모션 상태는 낙하 상태에서만 전환 가능
	if (NewState == ECharacterState::Locomotion)
	{
		return CurrentState == ECharacterState::Falling;
	}

	// 낙하 상태는 로코모션 상태에서만 전환 가능
	if (NewState == ECharacterState::Falling)
	{
		return CurrentState == ECharacterState::Locomotion;
	}

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
		CurrentHitSubState = EHitSubState::None;
		break;
	case ECharacterState::Locomotion:
	case ECharacterState::Falling:
	case ECharacterState::Dead:
	case ECharacterState::Skill:
		// 하위 상태가 없는 상태들
		break;
	}
}