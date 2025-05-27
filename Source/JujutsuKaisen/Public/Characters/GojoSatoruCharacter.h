// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JujutsuKaisenCharacter.h"
#include "GojoSatoruCharacter.generated.h"

/**
 * 
 */
UCLASS()
class JUJUTSUKAISEN_API AGojoSatoruCharacter : public AJujutsuKaisenCharacter
{
	GENERATED_BODY()

public:
	virtual void Skill_1() override;

	virtual void Skill_2() override;

	virtual void Skill_3() override;
	
};
