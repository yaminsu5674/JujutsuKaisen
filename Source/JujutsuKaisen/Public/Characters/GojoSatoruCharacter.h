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

protected:
	virtual void InitSkills() override;

	virtual void BeginPlay() override;

public:
	AGojoSatoruCharacter();

	virtual void Tick(float DeltaTime) override;
	
};
