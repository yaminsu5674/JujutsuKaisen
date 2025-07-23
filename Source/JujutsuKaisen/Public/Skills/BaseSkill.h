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

public:
	void SetOwner(AJujutsuKaisenCharacter* NewOwner);

	bool GetBWantsTick();

	virtual void TickSkill(float DeltaTime);

	virtual void OnPressed(AJujutsuKaisenCharacter* Target);

	virtual void OnReleased(AJujutsuKaisenCharacter* Target);
};
