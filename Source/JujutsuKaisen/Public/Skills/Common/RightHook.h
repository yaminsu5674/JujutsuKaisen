// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/BaseSkill.h"
#include "RightHook.generated.h"

/**
 * 
 */
UCLASS()
class JUJUTSUKAISEN_API URightHook : public UBaseSkill
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RightHookMontage;

public:
	URightHook();

	//virtual void Activate(AJujutsuKaisenCharacter* Caster, AJujutsuKaisenCharacter* Target = nullptr) override;
	
};
