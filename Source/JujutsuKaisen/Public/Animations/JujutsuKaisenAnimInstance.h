// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JujutsuKaisenAnimInstance.generated.h"


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Locomotion,
	Jump,
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	ECharacterState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JujutsuKaisenAnimInstance Params", meta = (AllowPrivateAccess = "true"))
	float Speed;

public:
	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();

	FORCEINLINE void SetState(ECharacterState InState) { State = InState; }

	FORCEINLINE ECharacterState GetState() const { return State; }

	FORCEINLINE void SetSpeed(float InSpeed) { Speed = InSpeed; }

	FORCEINLINE float GetSpeed() const { return Speed; }

};
