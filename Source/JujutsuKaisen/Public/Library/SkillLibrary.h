// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SkillLibrary.generated.h"

/**
 * 
 */
UCLASS()
class JUJUTSUKAISEN_API USkillLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// 타겟을 바라보도록 회전
	UFUNCTION(BlueprintCallable, Category = "Skill|Utility")
	static void RotateActorToFaceTarget(AActor* Source, AActor* Target);

	// 라인트레이스 추후 예정.
	UFUNCTION(BlueprintPure, Category = "Skill|Utility")
	static bool SimpleLineTrace(UObject* WorldContextObject, FVector Start, FVector End, FHitResult& OutHit);
	
};
