// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "JujutsuKaisenAIController.generated.h"

class AJujutsuKaisenCharacter;

/**
 * AI 컨트롤러 클래스
 * AI 행동을 관리하고 캐릭터를 제어하는 역할
 */
UCLASS()
class JUJUTSUKAISEN_API AJujutsuKaisenAIController : public AAIController
{
	GENERATED_BODY()

public:
	AJujutsuKaisenAIController();

	void InitializeBehaviorTree(TSoftObjectPtr<UBehaviorTree> BehaviorTreeAsset);

	void InitializeBlackboard();
	virtual void OnPossess(APawn* InPawn) override;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// AI 행동 업데이트 (임시 - 나중에 BehaviorTree로 대체)
	void UpdateAIBehavior(float DeltaTime);

	// AI용 함수들 (캐릭터의 AI 래핑 함수 호출)
	void AI_Move();
	void AI_Look();
	void AI_JumpCustom();
	void AI_Dash();
	void AI_StopDash();
	void AI_Guard();
	void AI_StopGuard();
	void AI_A_Pressed();
	void AI_R_Pressed();
	void AI_R_Released();

	// AI 상태 관리
	UPROPERTY(BlueprintReadOnly, Category = "AI State")
	bool bIsAIActive = false;

	UPROPERTY(BlueprintReadOnly, Category = "AI State")
	float AIUpdateInterval = 0.1f; // AI 업데이트 간격 (초)

	UPROPERTY(BlueprintReadOnly, Category = "AI State")
	float LastAIUpdateTime = 0.0f;

	// AI 행동 패턴 (임시)
	UPROPERTY(BlueprintReadOnly, Category = "AI Behavior")
	bool bShouldMove = true;

	UPROPERTY(BlueprintReadOnly, Category = "AI Behavior")
	bool bShouldJump = false;

	UPROPERTY(BlueprintReadOnly, Category = "AI Behavior")
	bool bShouldUseSkill = false;

	UPROPERTY(BlueprintReadOnly, Category = "AI Behavior")
	TSoftObjectPtr<UBehaviorTree> AssignedBehaviorTree;

	UPROPERTY()
	TWeakObjectPtr<AJujutsuKaisenCharacter> CachedAICharacter;
};
