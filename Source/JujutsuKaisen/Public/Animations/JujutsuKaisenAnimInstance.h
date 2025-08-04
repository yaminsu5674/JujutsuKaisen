// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JujutsuKaisenAnimInstance.generated.h"

class AJujutsuKaisenCharacter;

UENUM(BlueprintType)
enum class EAnimState : uint8
{
	Locomotion,
	Dash,
	Jump,
	Falling,
	Land,
	DoubleJump,
	SuperJump,
	Hit,
	Dead,
	Skill
};
/**
 *
 */
UCLASS()
class JUJUTSUKAISEN_API UJujutsuKaisenAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	UPROPERTY()
	AJujutsuKaisenCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	EAnimState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	float Speed;

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

	FORCEINLINE void SetState(EAnimState InState) { State = InState; }

	FORCEINLINE EAnimState GetState() const { return State; }

	//FORCEINLINE void SetSpeed(float InSpeed) { Speed = InSpeed; }

	//FORCEINLINE void SetBIsDahsing(bool result) { bIsDashing = result; }

	FORCEINLINE float GetSpeed() const { return Speed; }

};
