// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/JujutsuKaisenPlayerController.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

AJujutsuKaisenPlayerController::AJujutsuKaisenPlayerController()
{

	bEKeyPressed = false;
	bRKeyPressed = false;
}

void AJujutsuKaisenPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Add Input Mapping Context
	if (DefaultMappingContext)
	{
		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			if (auto* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				UE_LOG(LogTemp, Log, TEXT("Input Mapping Context added successfully"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("EnhancedInputLocalPlayerSubsystem not found"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LocalPlayer not found"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DefaultMappingContext is NULL! Please set it in PlayerController Blueprint"));
	}
	
	// 캐릭터를 플레이어 모드로 설정
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->SetPlayerMode(true);
		UE_LOG(LogTemp, Log, TEXT("Player Character set to Player Mode"));
	}
}

void AJujutsuKaisenPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UE_LOG(LogTemp, Log, TEXT("SetupInputComponent - DefaultMappingContext: %s"), DefaultMappingContext ? TEXT("Valid") : TEXT("NULL"));
	UE_LOG(LogTemp, Log, TEXT("SetupInputComponent - JumpAction: %s"), JumpAction ? TEXT("Valid") : TEXT("NULL"));
	
	// Enhanced Input Component 설정
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AJujutsuKaisenPlayerController::JumpCustom);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJujutsuKaisenPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJujutsuKaisenPlayerController::Look);

		// Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AJujutsuKaisenPlayerController::Dash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::StopDash);

		// Q_Pressed
		EnhancedInputComponent->BindAction(Q_Pressed_Action, ETriggerEvent::Started, this, &AJujutsuKaisenPlayerController::Q_Pressed);
		EnhancedInputComponent->BindAction(Q_Pressed_Action, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::Q_Released);

		// A_Pressed
		EnhancedInputComponent->BindAction(A_Pressed_Action, ETriggerEvent::Started, this, &AJujutsuKaisenPlayerController::A_Pressed);

		// R_Pressed (Started/Completed 트리거 사용)
		EnhancedInputComponent->BindAction(R_Pressed_Action, ETriggerEvent::Started, this, &AJujutsuKaisenPlayerController::R_Pressed);
		EnhancedInputComponent->BindAction(R_Pressed_Action, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::R_Released);
	
		// E_Pressed (Started/Completed 트리거 사용)
		EnhancedInputComponent->BindAction(E_Pressed_Action, ETriggerEvent::Started, this, &AJujutsuKaisenPlayerController::E_Pressed);
		EnhancedInputComponent->BindAction(E_Pressed_Action, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::E_Released);
	}
}

// Input Action Functions (래핑 함수들)
void AJujutsuKaisenPlayerController::Move(const FInputActionValue& Value)
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->Move(Value);
	}
}

void AJujutsuKaisenPlayerController::Look(const FInputActionValue& Value)
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->Look(Value);
	}
}

void AJujutsuKaisenPlayerController::JumpCustom()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->JumpCustom();
	}
}

void AJujutsuKaisenPlayerController::StopJumping()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->StopJumping();
	}
}

void AJujutsuKaisenPlayerController::Dash()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->Dash();
	}
}

void AJujutsuKaisenPlayerController::StopDash()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->StopDash();
	}
}

void AJujutsuKaisenPlayerController::Q_Pressed()
{
	bQKeyPressed = true;
    StartJujutsuComboTimer();
}

void AJujutsuKaisenPlayerController::Q_Released()
{
	GetWorld()->GetTimerManager().SetTimer(QKeyTimer, [this]() {
		bQKeyPressed = false;
		if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
		{
			Char->StopGuard();
		}
	}, KeyReleaseDelay, false);
}

void AJujutsuKaisenPlayerController::A_Pressed()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->A_Pressed();
	}
}

void AJujutsuKaisenPlayerController::R_Pressed()
{
	bRKeyPressed = true;
    StartJujutsuComboTimer();
}

void AJujutsuKaisenPlayerController::R_Released()
{
	GetWorld()->GetTimerManager().SetTimer(RKeyTimer, [this]() {
		bRKeyPressed = false;
		if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
		{
			Char->R_Released();
		}
	}, KeyReleaseDelay, false);
}

void AJujutsuKaisenPlayerController::E_Pressed()
{
	bEKeyPressed = true;
    StartJujutsuComboTimer();
}

void AJujutsuKaisenPlayerController::E_Released()
{
	GetWorld()->GetTimerManager().SetTimer(EKeyTimer, [this]() {
		bEKeyPressed = false;
	}, KeyReleaseDelay, false);

}

void AJujutsuKaisenPlayerController::StartJujutsuComboTimer()
{
    // 이전 타이머 있으면 리셋
    GetWorld()->GetTimerManager().ClearTimer(JujutsuComboTimer);

    // 새로 타이머 등록
    GetWorld()->GetTimerManager().SetTimer(
        JujutsuComboTimer,
        this,
        &AJujutsuKaisenPlayerController::JudgeJujutsuCombo,
        JujutsuComboDelay,
        false
    );
}

void AJujutsuKaisenPlayerController::JudgeJujutsuCombo()
{
    // ER 콤보 우선
    if (bEKeyPressed && bRKeyPressed)
    {
        if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
        {
            Char->ER_Pressed();
        }
    }
    // QR 콤보 우선
    else if (bQKeyPressed && bRKeyPressed)
    {
        if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
        {
            Char->QR_Pressed();
        }
    }
    // 단독 E
    else if (bEKeyPressed)
    {
        if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
        {
            Char->E_Pressed();
        }
    }
    // 단독 Q
    else if (bQKeyPressed)
    {
        if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
        {
            Char->Guard();
        }
    }
    // 단독 R
    else if (bRKeyPressed)
    {
        if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
        {
            Char->R_Pressed();
        }
    }
}



void AJujutsuKaisenPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// 캐릭터를 플레이어 모드로 설정
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(InPawn))
	{
		Char->SetPlayerMode(true);
		UE_LOG(LogTemp, Log, TEXT("Player Character set to Player Mode"));
	}
}
