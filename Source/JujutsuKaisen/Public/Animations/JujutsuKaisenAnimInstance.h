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
	Skill_1,
	Skill_2,
	Skill_3,
	// Punch
	// Kick
	// ...
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

	UFUNCTION(BlueprintCallable)
	void SetState(ECharacterState InState);

	UFUNCTION(BlueprintCallable)
	ECharacterState GetState() const;

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float InSpeed);

	UFUNCTION(BlueprintCallable)
	float GetSpeed() const;

};
