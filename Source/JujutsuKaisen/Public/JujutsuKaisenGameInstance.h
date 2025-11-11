// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"
#include "BehaviorTree/BehaviorTree.h"
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

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UJujutsuKaisenCharacterDataAsset* MyCharacterDataAsset;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UJujutsuKaisenCharacterDataAsset* EnemyCharacterDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UBehaviorTree> SelectedBehaviorTree;


public:
	UFUNCTION(BlueprintCallable, Category = "JujutsuKaisen")
	void StartSingle();

	UFUNCTION(BlueprintCallable, Category = "JujutsuKaisen")
	void LeaveSingle();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void SetMyCharacterByPath(const FString& AssetPath);

	UFUNCTION(BlueprintCallable)
	void SetEnemyCharacterByPath(const FString& AssetPath);

	FORCEINLINE UJujutsuKaisenCharacterDataAsset* GetMyCharacterDataAsset() const { return MyCharacterDataAsset; }

	FORCEINLINE UJujutsuKaisenCharacterDataAsset* GetEnemyCharacterDataAsset() const { return EnemyCharacterDataAsset; }

	FORCEINLINE TSoftObjectPtr<UBehaviorTree> GetSelectedBehaviorTree() const { return SelectedBehaviorTree; }
	
	FORCEINLINE void SetSelectedBehaviorTree(TSoftObjectPtr<UBehaviorTree> InBehaviorTree) { SelectedBehaviorTree = InBehaviorTree; }
	
};
