// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skills/BaseSkill.h"
#include "SkillManager.generated.h"

/**
 * 
 */
class AJujutsuKaisenCharacter;


UCLASS()
class JUJUTSUKAISEN_API USkillManager : public UObject
{
	GENERATED_BODY()

    UPROPERTY()
    TMap<FName, UBaseSkill*> BoundSkills;

    UPROPERTY()
    AJujutsuKaisenCharacter* Owner;

public:
    void RegisterSkill(FName Key, UBaseSkill* Skill);

    void RegisterOwner(AJujutsuKaisenCharacter* InOwner);

    void HandlePressed(FName Key, AJujutsuKaisenCharacter* Target);

    void HandleReleased(FName Key, AJujutsuKaisenCharacter* Target);

    void TickActiveSkills(float Delta);
	
};
