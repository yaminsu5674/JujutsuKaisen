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
	FString Name;

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

	// ĸ�� ������Ʈ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	double HalfHeight = 1.0f;

	// ĸ�� ������Ʈ �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	double Radius = 1.0f;

	// LeftFist Name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	FString LeftFistName;

	// RightFist Name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	FString RightFistName;

	// LeftFoot Name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	FString LeftFootName;

	// RightFoot Name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	FString RightFootName;



public:
	FORCEINLINE TSubclassOf<AJujutsuKaisenCharacter> GetCharacterClass() const { return CharacterClass; }

	FORCEINLINE USkeletalMesh* GetMesh() const { return Mesh; }

	FORCEINLINE double GetMeshScale() const { return MeshScale; }

	FORCEINLINE TSubclassOf<UAnimInstance> GetAnimBP() const { return AnimBP; }

	FORCEINLINE double GetHalfHeight() const { return HalfHeight; }

	FORCEINLINE double GetRadius() const { return Radius; }

	FORCEINLINE FString GetLeftFistName() const { return LeftFistName; }

	FORCEINLINE FString GetRightFistName() const { return RightFistName; }

	FORCEINLINE FString GetLeftFootName() const { return LeftFootName; }

	FORCEINLINE FString GetRightFootName() const { return RightFootName; }



};
