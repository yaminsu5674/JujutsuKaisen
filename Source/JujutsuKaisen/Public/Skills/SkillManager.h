// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skills/BaseSkill.h"
#include "SkillManager.generated.h"

/**
 * 
 */
class AJujutsuKaisenCharacter;
class UCameraAnimationSequence;

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class JUJUTSUKAISEN_API USkillManager : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Skill", meta = (AllowPrivateAccess = "true"))
    TMap<FName, UBaseSkill*> BoundSkills;

public:
	// 카메라 쉐이크 시작 (인스턴스 멤버 - 각 캐릭터별로 독립적)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraShakeStart);
	UPROPERTY()
	FOnCameraShakeStart OnCameraShakeStartEvent;

	// 카메라 쉐이크 종료
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraShakeEnd);
	UPROPERTY()
	FOnCameraShakeEnd OnCameraShakeEndEvent;

	// 카메라 애니메이션 시작
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraAnimationStart, UCameraAnimationSequence*, CameraAnim);
	UPROPERTY()
	FOnCameraAnimationStart OnCameraAnimationStartEvent;

	// 카메라 애니메이션 종료
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraAnimationEnd);
	UPROPERTY()
	FOnCameraAnimationEnd OnCameraAnimationEndEvent;

public:
    void HandlePressed(FName Key);

    void HandleReleased(FName Key);

    void TickActiveSkills(float Delta);

    void ResetActiveSkills();

    TArray<FName> GetRegisteredSkillNames() const;
	
};
