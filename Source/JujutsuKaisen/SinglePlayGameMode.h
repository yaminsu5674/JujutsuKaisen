// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JujutsuKaisenGameMode.h"
#include "JujutsuKaisenCharacter.h"
#include "JujutsuKaisenCharacterDataAsset.h"
#include "SinglePlayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JUJUTSUKAISEN_API ASinglePlayGameMode : public AJujutsuKaisenGameMode
{
	GENERATED_BODY()

	AJujutsuKaisenCharacter* PlayerCharacter = NULL;

	AJujutsuKaisenCharacter* EnemyCharacter = NULL;

protected:
	virtual void BeginPlay() override;

	void SpawnCharacterFromData(UJujutsuKaisenCharacterDataAsset* DataAsset, const FVector& SpawnLocation, const FRotator& SpawnRotation, bool bIsPlayerCharacter);
	
	void InitCharacterFromData(UJujutsuKaisenCharacterDataAsset* DataAsset, bool bIsPlayerCharacter);

};
