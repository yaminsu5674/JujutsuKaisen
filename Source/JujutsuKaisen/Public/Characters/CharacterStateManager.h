// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStateManager.generated.h"

class AJujutsuKaisenCharacter;

// 메인 상태 (우선순위 순서)
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Locomotion = 0,     // 로코모션 (최고 우선순위)
	Falling = 1,        // 낙하
	Skill = 2,          // 스킬 사용
	Hit = 3             // 피격
};

// 피격 하위 상태
UENUM(BlueprintType)
enum class EHitSubState : uint8
{
	Custom = 0,
	Stun = 1,
	Light = 2,
	KnockBack = 3
};


/**
 * 캐릭터 상태 관리 클래스
 * 메인 상태와 하위 상태를 관리하며 상태 전환 우선순위를 처리
 */
UCLASS(BlueprintType)
class JUJUTSUKAISEN_API UCharacterStateManager : public UObject
{
	GENERATED_BODY()

public:
	UCharacterStateManager();

	// 캐릭터 참조 설정
	void SetOwnerCharacter(AJujutsuKaisenCharacter* Character);

	// 메인 상태
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECharacterState CurrentState = ECharacterState::Locomotion;

	// 하위 상태들
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EHitSubState CurrentHitSubState;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool IsHitFront = true;

	// 상태 전환 함수들
	UFUNCTION(BlueprintCallable)
	bool SetState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable)
	bool SetHitSubState(EHitSubState NewSubState, bool bIsHitFront = true);

	// 상태 체크 함수들
	UFUNCTION(BlueprintCallable)
	bool IsInState(ECharacterState State) const;

	UFUNCTION(BlueprintCallable)
	bool CanTransitionTo(ECharacterState NewState) const;

	// Getter 함수들
	UFUNCTION(BlueprintCallable)
	ECharacterState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable)
	EHitSubState GetCurrentHitSubState() const { return CurrentHitSubState; }

	UFUNCTION(BlueprintCallable)
	bool GetIsHitFront() const { return IsHitFront; }

	// 강제 상태 설정 (우선순위 무시)
	UFUNCTION(BlueprintCallable)
	void ForceState(ECharacterState NewState);

private:
	// 캐릭터 참조
	UPROPERTY()
	AJujutsuKaisenCharacter* OwnerCharacter = nullptr;

	// 상태 전환 시 하위 상태 리셋
	void ResetSubStates(ECharacterState NewState);

	// 상태 전환 우선순위 체크
	bool CheckStatePriority(ECharacterState NewState) const;

	// 중력 제어
	void UpdateGravityForState(ECharacterState NewState);
};
