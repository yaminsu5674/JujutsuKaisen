// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
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

public:
	// 캐릭터 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	FText Name;

	// 캐릭터 클래스 (예: 고조사토루, 스쿠나 등의 서브클래스)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	TSubclassOf<AJujutsuKaisenCharacter> CharacterClass;

	// 스켈레탈 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	USkeletalMesh* Mesh;

	// 메시 스케일
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	float MeshScale = 1.0f;

	// 애니메이션 블루프린트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData")
	TSubclassOf<UAnimInstance> AnimBP;
};
