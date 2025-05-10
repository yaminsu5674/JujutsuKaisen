// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "JujutsuKaisenCharacterDataAsset.generated.h"

// �̸� ���� (��� �� ��ȯ ���� ����)
class AJujutsuKaisenCharacter;
class USkeletalMesh;
class UAnimInstance;

/**
 * ���������, ���� ���� ĳ���� ������ ��� ������ ���� Ŭ����
 */
UCLASS(BlueprintType)
class JUJUTSUKAISEN_API UJujutsuKaisenCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ĳ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	FText Name;

	// ĳ���� Ŭ���� (��: ���������, ���� ���� ����Ŭ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	TSubclassOf<AJujutsuKaisenCharacter> CharacterClass;

	// ���̷�Ż �޽�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	USkeletalMesh* Mesh;

	// �޽� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	float MeshScale = 1.0f;

	// �ִϸ��̼� �������Ʈ Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	TSubclassOf<UAnimInstance> AnimBP;
};
