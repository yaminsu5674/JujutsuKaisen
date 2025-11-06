// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "SkillLibrary.generated.h"

/**
 * 
 */
UCLASS()
class JUJUTSUKAISEN_API USkillLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Ÿ���� �ٶ󺸵��� ȸ��
	UFUNCTION(BlueprintCallable, Category = "Skill|Utility")
	static void RotateActorToFaceTarget(AActor* Source, AActor* Target);

	// ����Ʈ���̽� ���� ����.
	UFUNCTION(BlueprintPure, Category = "Skill|Utility")
	static bool SimpleLineTrace(UObject* WorldContextObject, FVector Start, FVector End, FHitResult& OutHit);
	
	// 재귀적으로 Owner를 타고 올라가서 JujutsuKaisenCharacter 찾기
	UFUNCTION(BlueprintCallable, Category = "Skill|Utility")
	static AJujutsuKaisenCharacter* GetOwnerRecursive(UObject* Object);
	
	// 공격 방향 판단 (앞/뒤)
	UFUNCTION(BlueprintCallable, Category = "Skill|Utility")
	static bool JudgeHitFront(AActor* Attacker, AActor* Victim);
	
};
