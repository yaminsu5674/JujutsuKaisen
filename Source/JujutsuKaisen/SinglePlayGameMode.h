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

	UPROPERTY(EditAnywhere, Category = "Character params")
	FVector PlayerSpawnLocation = FVector(0.f, -200.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Character params")
	FVector EnemySpawnLocation = FVector(1000.f, -200.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Character params")
	FRotator PlayerSpawnRotation = FRotator(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Character params")
	FRotator EnemySpawnRotation = FRotator(0.f, 180.f, 0.f);

	

public:
	ASinglePlayGameMode();

protected:
	virtual void BeginPlay() override;

	void SpawnCharacterFromData(UJujutsuKaisenCharacterDataAsset* DataAsset, const FVector& SpawnLocation, const FRotator& SpawnRotation, bool bIsPlayerCharacter);

	void PossessPlayer();

	void PossessAI();
};
