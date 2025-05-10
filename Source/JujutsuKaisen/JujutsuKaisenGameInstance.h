// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JujutsuKaisenCharacterDataAsset.h"
#include "JujutsuKaisenGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class JUJUTSUKAISEN_API UJujutsuKaisenGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// my player data asset store as text 

	// enemy player data asset store as text

public:
	UPROPERTY(BlueprintReadOnly)
	UJujutsuKaisenCharacterDataAsset* MyCharacterDataAsset;

	UPROPERTY(BlueprintReadOnly)
	UJujutsuKaisenCharacterDataAsset* EnemyCharacterDataAsset;


public:
	UFUNCTION(BlueprintCallable, Category = "JujutsuKaisen")
	void StartSingle();

	UFUNCTION(BlueprintCallable, Category = "JujutsuKaisen")
	void LeaveGame();

	virtual void Init() override;
	
};
