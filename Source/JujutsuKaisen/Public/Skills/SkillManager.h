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


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class JUJUTSUKAISEN_API USkillManager : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Skill", meta = (AllowPrivateAccess = "true"))
    TMap<FName, UBaseSkill*> BoundSkills;

    UPROPERTY()
    AJujutsuKaisenCharacter* Owner;

public:
    void InitAllSkillOwner();

    void RegisterOwner(AJujutsuKaisenCharacter* InOwner);

    void HandlePressed(FName Key);

    void HandleReleased(FName Key);

    void TickActiveSkills(float Delta);

    void ResetActiveSkills();
	
};
