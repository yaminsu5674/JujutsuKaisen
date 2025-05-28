// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Character.h"
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

	// ĳ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	FText Name;

	// ĳ���� Ŭ���� (��: ���������, ���� ���� ����Ŭ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AJujutsuKaisenCharacter> CharacterClass;

	// ���̷�Ż �޽�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* Mesh;

	// �޽� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	double MeshScale = 1.0f;

	// �ִϸ��̼� �������Ʈ Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> AnimBP;

public:
	// Getter for CharacterClass
	FORCEINLINE TSubclassOf<AJujutsuKaisenCharacter> GetCharacterClass() const { return CharacterClass; }

	// Getter for Mesh
	FORCEINLINE USkeletalMesh* GetMesh() const { return Mesh; }

	// Getter for MeshScale
	FORCEINLINE double GetMeshScale() const { return MeshScale; }

	// Getter for AnimBP
	FORCEINLINE TSubclassOf<UAnimInstance> GetAnimBP() const { return AnimBP; }

};
