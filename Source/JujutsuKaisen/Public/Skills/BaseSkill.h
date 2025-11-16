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
	bool bWantsTick; // 필요시에만 Tick

	// TODO: 카메라 애니메이션 기능은 플러그인 활성화 후 재도입 예정
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	// UCameraAnimationSequence* CameraAnimation = nullptr;

	// 헬퍼 함수: 재귀적으로 Owner 찾기
	AJujutsuKaisenCharacter* GetOwner() const;
	
	// 헬퍼 함수: Owner를 통해 Target 가져오기
	AJujutsuKaisenCharacter* GetTarget() const;
	
	// 헬퍼 함수: Owner를 통해 AnimInstance 가져오기
	UAnimInstance* GetAnimInstance() const;

public:
	UBaseSkill();

	bool GetBWantsTick();

	virtual void ResetSkill();

	virtual void TickSkill(float DeltaTime);

	virtual void OnPressed();

	virtual void OnReleased();
};
