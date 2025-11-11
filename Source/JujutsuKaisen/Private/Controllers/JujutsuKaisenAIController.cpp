// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/JujutsuKaisenAIController.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Engine/World.h"
#include "InputActionValue.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	CachedAICharacter = nullptr;
	DesiredEngageDistance = FMath::RandRange(200.0f, 3000.0f);
	bIsWaiting = false;
	WaitTimer = 0.0f;
}

void AJujutsuKaisenAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AJujutsuKaisenAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedAICharacter = Cast<AJujutsuKaisenCharacter>(InPawn);
}

void AJujutsuKaisenAIController::InitializeBehaviorTree(TSoftObjectPtr<UBehaviorTree> BehaviorTreeAsset)
{
	// 추후 비헤비어트리 작업하며 되살릴 것임.
	// if (BehaviorTreeAsset.IsNull())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("InitializeBehaviorTree: BehaviorTreeAsset is null."));
	// 	return;
	// }

	// UBehaviorTree* LoadedBehaviorTree = BehaviorTreeAsset.LoadSynchronous();
	// if (!LoadedBehaviorTree)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("InitializeBehaviorTree: Failed to load behavior tree asset."));
	// 	return;
	// }

	// if (LoadedBehaviorTree->BlackboardAsset)
	// {
	// 	UBlackboardComponent* BlackboardComp = nullptr;
	// 	if (!UseBlackboard(LoadedBehaviorTree->BlackboardAsset, BlackboardComp))
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("InitializeBehaviorTree: UseBlackboard failed."));
	// 	}
	// }

	// if (!RunBehaviorTree(LoadedBehaviorTree))
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("InitializeBehaviorTree: RunBehaviorTree failed."));
	// 	return;
	// }

	// AssignedBehaviorTree = BehaviorTreeAsset;
	// CachedAICharacter = Cast<AJujutsuKaisenCharacter>(GetPawn());
	// UE_LOG(LogTemp, Warning, TEXT("InitializeBehaviorTree: Behavior tree '%s' started."), *LoadedBehaviorTree->GetName());
	// InitializeBlackboard();
}

void AJujutsuKaisenAIController::InitializeBlackboard()
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		if (!CachedAICharacter.IsValid())
		{
			CachedAICharacter = Cast<AJujutsuKaisenCharacter>(GetPawn());
		}

		if (CachedAICharacter.IsValid())
		{
			if (AJujutsuKaisenCharacter* Target = CachedAICharacter->GetTargetCharacter())
			{
				BlackboardComp->SetValueAsVector(TEXT("TargetLocation"), Target->GetActorLocation());
			}
			else
			{
				BlackboardComp->ClearValue(TEXT("TargetLocation"));
			}
		}
	}
}

void AJujutsuKaisenAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 추후 재작업 예정. 현재는 임시로 여기서 직접 ai 간단 제어.
	// if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	// {
	// 	if (CachedAICharacter.IsValid())
	// 	{
	// 		if (AJujutsuKaisenCharacter* Target = CachedAICharacter->GetTargetCharacter())
	// 		{
	// 			BlackboardComp->SetValueAsVector(TEXT("TargetLocation"), Target->GetActorLocation());
	// 		}
	// 	}
	// }
	
	UpdateAIBehavior(DeltaTime);
	
	// if (bIsAIActive)
	
}

void AJujutsuKaisenAIController::UpdateAIBehavior(float DeltaTime)
{
	if (!CachedAICharacter.IsValid())
	{
		CachedAICharacter = Cast<AJujutsuKaisenCharacter>(GetPawn());
	}

	AJujutsuKaisenCharacter* AICharacter = CachedAICharacter.Get();
	if (!AICharacter || AICharacter->GetIsDead())
	{
		StopMovement();
		return;
	}

	AJujutsuKaisenCharacter* TargetCharacter = AICharacter->GetTargetCharacter();
	if (!TargetCharacter || TargetCharacter->GetIsDead())
	{
		StopMovement();
		return;
	}

	if (bIsWaiting)
	{
		WaitTimer -= DeltaTime;
		if (WaitTimer <= 0.0f)
		{
			bIsWaiting = false;
			DesiredEngageDistance = FMath::RandRange(200.0f, 3000.0f);
		}
		else
		{
			StopMovement();
			return;
		}
	}

	UCharacterStateManager* StateManager = AICharacter->GetStateManager();
	if (!StateManager)
	{
		StopMovement();
		return;
	}

	const float DistanceToTarget = FVector::Dist(TargetCharacter->GetActorLocation(), AICharacter->GetActorLocation());
	const bool bWithinDesiredRange = DistanceToTarget <= DesiredEngageDistance;
	const float AcceptanceRadius = 150.0f;
	bool bIsMoving = false;

	if (!bWithinDesiredRange)
	{
		if (!StateManager->IsInState(ECharacterState::Locomotion))
		{
			StopMovement();
			return;
		}

		if (!bIsMoving)
		{
			MoveToActor(TargetCharacter, AcceptanceRadius, true, true, true, nullptr, true);
			bIsMoving = true;
		}

		return;
	}

	StopMovement();

	if (!StateManager->CanTransitionTo(ECharacterState::Skill))
	{
		bIsWaiting = true;
		WaitTimer = FMath::RandRange(1.0f, 2.0f);
		return;
	}

	USkillManager* SkillManager = AICharacter->GetSkillManager();
	if (!SkillManager)
	{
		return;
	}

	TArray<FName> RegisteredSkills = SkillManager->GetRegisteredSkillNames();
	RegisteredSkills.RemoveAll([](const FName& SkillName)
	{
		return SkillName.IsNone();
	});

	if (RegisteredSkills.Num() == 0)
	{
		return;
	}

	const int32 SkillIndex = FMath::RandRange(0, RegisteredSkills.Num() - 1);
	const FName SelectedSkillName = RegisteredSkills[SkillIndex];

	if (SelectedSkillName == FName(TEXT("R")))
	{
		AI_R_Pressed();
		AI_R_Released();
	}
	else if (SelectedSkillName == FName(TEXT("ER")))
	{
		AI_ER_Pressed();
	}
	else if (SelectedSkillName == FName(TEXT("E")))
	{
		AI_E_Pressed();
	}
	else
	{
		SkillManager->HandlePressed(SelectedSkillName);
		SkillManager->HandleReleased(SelectedSkillName);
	}

	bIsWaiting = true;
	WaitTimer = FMath::RandRange(1.0f, 2.0f);
	DesiredEngageDistance = FMath::RandRange(200.0f, 3000.0f);
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

void AJujutsuKaisenAIController::AI_E_Pressed()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->E_Pressed();
	}
}

void AJujutsuKaisenAIController::AI_ER_Pressed()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->ER_Pressed();
	}
}

