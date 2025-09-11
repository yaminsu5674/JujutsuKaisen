// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/JujutsuKaisenAIController.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Engine/World.h"
#include "InputActionValue.h"

AJujutsuKaisenAIController::AJujutsuKaisenAIController()
{
	// 기본 설정
	bIsAIActive = true;
	AIUpdateInterval = 0.1f;
	LastAIUpdateTime = 0.0f;
	
	// AI 행동 패턴 초기화
	bShouldMove = true;
	bShouldJump = false;
	bShouldUseSkill = false;
}

void AJujutsuKaisenAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AJujutsuKaisenAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// if (bIsAIActive)
	// {
	// 	// AI 업데이트 간격 체크
	// 	LastAIUpdateTime += DeltaTime;
	// 	if (LastAIUpdateTime >= AIUpdateInterval)
	// 	{
	// 		UpdateAIBehavior(DeltaTime);
	// 		LastAIUpdateTime = 0.0f;
	// 	}
	// }
}

void AJujutsuKaisenAIController::UpdateAIBehavior(float DeltaTime)
{
	if (!GetPawn())
	{
		return;
	}
	
	// 간단한 AI 행동 로직 (임시)
	// 나중에 BehaviorTree로 대체될 예정
	
	// 이동 로직
	if (bShouldMove)
	{
		AI_Move();
	}
	
	// 점프 로직 (랜덤하게)
	if (FMath::RandRange(0.0f, 1.0f) < 0.01f) // 1% 확률로 점프
	{
		bShouldJump = true;
	}
	
	if (bShouldJump)
	{
		AI_JumpCustom();
		bShouldJump = false;
	}
	
	// 스킬 사용 로직 (랜덤하게)
	if (FMath::RandRange(0.0f, 1.0f) < 0.005f) // 0.5% 확률로 스킬 사용
	{
		bShouldUseSkill = true;
	}
	
	if (bShouldUseSkill)
	{
		// A키 또는 R키 스킬 랜덤 선택
		if (FMath::RandRange(0, 1) == 0)
		{
			AI_A_Pressed();
		}
		else
		{
			AI_R_Pressed();
		}
		bShouldUseSkill = false;
	}
}

// AI용 함수들 (캐릭터의 함수 직접 호출)
void AJujutsuKaisenAIController::AI_Move()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		// AI용 이동 로직 (기본값으로 전진)
		FVector2D MovementVector = FVector2D(1.0f, 0.0f);
		FInputActionValue Value(MovementVector);
		Char->Move(Value);
	}
}

void AJujutsuKaisenAIController::AI_Look()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		// AI용 시점 로직 (기본값으로 정면)
		FVector2D LookVector = FVector2D(0.0f, 0.0f);
		FInputActionValue Value(LookVector);
		Char->Look(Value);
	}
}

void AJujutsuKaisenAIController::AI_JumpCustom()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->JumpCustom();
	}
}

void AJujutsuKaisenAIController::AI_Dash()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->Dash();
	}
}

void AJujutsuKaisenAIController::AI_StopDash()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->StopDash();
	}
}

void AJujutsuKaisenAIController::AI_Guard()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->Guard();
	}
}

void AJujutsuKaisenAIController::AI_StopGuard()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->StopGuard();
	}
}

void AJujutsuKaisenAIController::AI_A_Pressed()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->A_Pressed();
	}
}

void AJujutsuKaisenAIController::AI_R_Pressed()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->R_Pressed();
	}
}

void AJujutsuKaisenAIController::AI_R_Released()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->R_Released();
	}
}

