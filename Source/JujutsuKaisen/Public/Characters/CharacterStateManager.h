// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStateManager.generated.h"

// 메인 상태 (우선순위 순서)
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Dead = 0,           // 죽음 (최고 우선순위)
	Hit = 1,            // 피격
	Skill = 2,          // 스킬 사용
	Falling = 3,        // 낙하
	Locomotion = 4      // 로코모션
};

// 피격 하위 상태
UENUM(BlueprintType)
enum class EHitSubState : uint8
{
	None = 0,
	LightHit = 1,       // 약한 피격
	MediumHit = 2,      // 중간 피격
	HeavyHit = 3,       // 강한 피격
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

	// 메인 상태
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECharacterState CurrentState = ECharacterState::Locomotion;

	// 하위 상태들
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EHitSubState CurrentHitSubState = EHitSubState::None;

	// 상태 전환 함수들
	UFUNCTION(BlueprintCallable)
	bool SetState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable)
	bool SetHitSubState(EHitSubState NewSubState);

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

	// 강제 상태 설정 (우선순위 무시)
	UFUNCTION(BlueprintCallable)
	void ForceState(ECharacterState NewState);

private:
	// 상태 전환 시 하위 상태 리셋
	void ResetSubStates(ECharacterState NewState);

	// 상태 전환 우선순위 체크
	bool CheckStatePriority(ECharacterState NewState) const;
};
