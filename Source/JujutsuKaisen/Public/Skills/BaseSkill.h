// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseSkill.generated.h"


/**
 * 
 */
class AJujutsuKaisenCharacter;

UCLASS()
class JUJUTSUKAISEN_API UBaseSkill : public UObject
{
	GENERATED_BODY()

protected:
	bool bWantsTick = false; // 필요할 때만 Tick

	UPROPERTY()
	AJujutsuKaisenCharacter* Owner;

	UPROPERTY()
	AJujutsuKaisenCharacter* Target;

	UPROPERTY()
	UAnimInstance* AnimInstance;

public:
	void SetOwner(AJujutsuKaisenCharacter* NewOwner);

	void SetTarget(AJujutsuKaisenCharacter* NewTarget);

	bool GetBWantsTick();

	virtual void ResetSkill();

	virtual void TickSkill(float DeltaTime);

	virtual void OnPressed();

	virtual void OnReleased();
};
