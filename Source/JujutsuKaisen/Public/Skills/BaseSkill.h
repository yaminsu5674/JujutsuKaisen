// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "BaseSkill.generated.h"


/**
 * 
 */
UCLASS()
class JUJUTSUKAISEN_API UBaseSkill : public UObject
{
	GENERATED_BODY()

public:
	virtual void Activate(AJujutsuKaisenCharacter* Caster, AJujutsuKaisenCharacter* Target = nullptr);
};
