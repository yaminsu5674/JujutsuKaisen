// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseSkill.generated.h"


/**
 * 
 */
class AJujutsuKaisenCharacter;

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class JUJUTSUKAISEN_API UBaseSkill : public UObject
{
	GENERATED_BODY()

protected:
	bool bWantsTick = false; // 필요시에만 Tick

	// 헬퍼 함수: 재귀적으로 Owner 찾기
	AJujutsuKaisenCharacter* GetOwner() const;
	
	// 헬퍼 함수: Owner를 통해 Target 가져오기
	AJujutsuKaisenCharacter* GetTarget() const;
	
	// 헬퍼 함수: Owner를 통해 AnimInstance 가져오기
	UAnimInstance* GetAnimInstance() const;

public:
	bool GetBWantsTick();

	virtual void ResetSkill();

	virtual void TickSkill(float DeltaTime);

	virtual void OnPressed();

	virtual void OnReleased();
};
