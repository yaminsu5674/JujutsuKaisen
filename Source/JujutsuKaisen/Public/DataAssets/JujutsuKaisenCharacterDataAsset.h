// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Character.h"
#include "JujutsuKaisenCharacterDataAsset.generated.h"

// 미리 선언 (헤더 간 순환 참조 방지)
class AJujutsuKaisenCharacter;
class USkeletalMesh;
class UAnimInstance;

/**
 * 고조사토루, 스쿠나 등의 캐릭터 정보를 담는 데이터 에셋 클래스
 */
UCLASS(BlueprintType)
class JUJUTSUKAISEN_API UJujutsuKaisenCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	// 캐릭터 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	FString Name;

	// 캐릭터 클래스 (예: 고조사토루, 스쿠나 등의 서브클래스)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AJujutsuKaisenCharacter> CharacterClass;

	// 스켈레탈 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* Mesh;

	// 메시 스케일
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	double MeshScale = 1.0f;

	// 애니메이션 블루프린트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> AnimBP;

	// 캡슐 컴포넌트 절반 높이
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	double HalfHeight = 1.0f;

	// 캡슐 컴포넌트 반경
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
