// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterStateManager.h"
#include "JujutsuKaisenAnimInstance.generated.h"

class AJujutsuKaisenCharacter;

/**
 * 애니메이션 인스턴스 클래스
 * 상태 매니저의 상태들을 사용하여 애니메이션을 관리
 */
UCLASS()
class JUJUTSUKAISEN_API UJujutsuKaisenAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY()
	AJujutsuKaisenCharacter* Character;

	// 상태 매니저의 상태들 (블루프린트에서 접근 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	ECharacterState CurrentState = ECharacterState::Locomotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	EHitSubState CurrentHitSubState = EHitSubState::Custom;

	// 기존 애니메이션 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	float VerticalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	bool bIsHitFront;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	bool bIsDashing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	uint8 JumpCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	bool bDidSuperJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	bool bDidDoubleJump;


protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeInitializeAnimation() override;

public:
	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();

	// 상태 관련 Getter 함수들
	FORCEINLINE ECharacterState GetCurrentState() const { return CurrentState; }
	FORCEINLINE EHitSubState GetCurrentHitSubState() const { return CurrentHitSubState; }

	FORCEINLINE float GetSpeed() const { return Speed; }

};
