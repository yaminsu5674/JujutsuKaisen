// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Character.h"
#include "JujutsuKaisenCharacterDataAsset.generated.h"

// �̸� ���� (��� �� ��ȯ ���� ����)
class AJujutsuKaisenCharacter;

/**
 * ���������, ������ ���� ĳ���� ������ ��� ������ ���� Ŭ����
 */
UCLASS(BlueprintType)
class JUJUTSUKAISEN_API UJujutsuKaisenCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	// ĳ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	FString Name;

	// ĳ���� Ŭ���� (��: ���������, ������ ���� ����Ŭ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AJujutsuKaisenCharacter> CharacterClass;

	





public:
	FORCEINLINE TSubclassOf<AJujutsuKaisenCharacter> GetCharacterClass() const { return CharacterClass; }
	FORCEINLINE const FString& GetCharacterName() const { return Name; }
};
