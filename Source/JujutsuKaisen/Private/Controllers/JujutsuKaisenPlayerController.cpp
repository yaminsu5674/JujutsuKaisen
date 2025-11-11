// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/JujutsuKaisenPlayerController.h"
#include "Controllers/CustomCameraManager.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/PlayerHealthBarWidget.h"
#include "UI/EnemyHealthBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/SinglePlayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "JujutsuKaisenGameInstance.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"

AJujutsuKaisenPlayerController::AJujutsuKaisenPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerCameraManagerClass = ACustomCameraManager::StaticClass();
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

		CachedPlayerCharacter = Char;
	}

	RefreshEnemyReference();
	InitializeHUD();
	UpdateHUDData();
}

void AJujutsuKaisenPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input Component 설정
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AJujutsuKaisenPlayerController::JumpCustom);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJujutsuKaisenPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::MoveCompleted);

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
		EnhancedInputComponent->BindAction(R_Pressed_Action, ETriggerEvent::Triggered, this, &AJujutsuKaisenPlayerController::R_Pressed);
		EnhancedInputComponent->BindAction(R_Pressed_Action, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::R_Released);
	
		// E_Pressed (Started/Completed 트리거 사용)
		EnhancedInputComponent->BindAction(E_Pressed_Action, ETriggerEvent::Started, this, &AJujutsuKaisenPlayerController::E_Pressed);
		EnhancedInputComponent->BindAction(E_Pressed_Action, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::E_Released);

		// ER Chord
		EnhancedInputComponent->BindAction(ER_Chord_Action, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::ER_Chord);

		// QR Chord
		EnhancedInputComponent->BindAction(QR_Chord_Action, ETriggerEvent::Completed, this, &AJujutsuKaisenPlayerController::QR_Chord);
	}
}

// Input Action Functions (래핑 함수들)
void AJujutsuKaisenPlayerController::Move(const FInputActionValue& Value)
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->Move(Value);
		Char->SetIsMoving(true);
	}
}

void AJujutsuKaisenPlayerController::MoveCompleted()
{

	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->SetIsMoving(false);
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
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->Guard();
	}
}

void AJujutsuKaisenPlayerController::Q_Released()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->StopGuard();
	}
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
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->R_Pressed();
	}
}

void AJujutsuKaisenPlayerController::R_Released()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->R_Released();
	}
}

void AJujutsuKaisenPlayerController::E_Pressed()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->E_Pressed();
	}
}

void AJujutsuKaisenPlayerController::E_Released()
{
	// 현재는 래핑할 대상이 없음
}

void AJujutsuKaisenPlayerController::ER_Chord()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->ER_Pressed();
	}
}

void AJujutsuKaisenPlayerController::QR_Chord()
{
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->QR_Pressed();
	}
}

void AJujutsuKaisenPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// 캐릭터를 플레이어 모드로 설정
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(InPawn))
	{
		Char->SetPlayerMode(true);
		UE_LOG(LogTemp, Error, TEXT("Player Character set to Player Mode (Class: %s)"), *Char->GetClass()->GetName());
		CachedPlayerCharacter = Char;
		RefreshEnemyReference();
		UpdateHUDData();
	}
}

void AJujutsuKaisenPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	RefreshEnemyReference();
	UpdateHUDData();

	if (PlayerHUDWidgetInstance && CachedEnemyCharacter.IsValid())
	{
		PlayerHUDWidgetInstance->UpdateEnemyHealthBarForCharacter(CachedEnemyCharacter.Get());
	}
}

void AJujutsuKaisenPlayerController::InitializeHUD()
{
	if (PlayerHUDWidgetInstance || !PlayerHUDWidgetClass)
	{
		return;
	}

	PlayerHUDWidgetInstance = CreateWidget<UPlayerHUDWidget>(this, PlayerHUDWidgetClass);

	if (!PlayerHUDWidgetInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create PlayerHUDWidget instance."));
		return;
	}

	PlayerHUDWidgetInstance->AddToViewport();

	if (UPlayerHealthBarWidget* PlayerBar = PlayerHUDWidgetInstance->GetPlayerHealthBar())
	{
		if (CachedPlayerCharacter.IsValid())
		{
			PlayerBar->InitializeWithCharacter(CachedPlayerCharacter.Get());
		}
	}

	if (UEnemyHealthBarWidget* EnemyBar = PlayerHUDWidgetInstance->GetEnemyHealthBar())
	{
		if (CachedEnemyCharacter.IsValid())
		{
			EnemyBar->InitializeWithCharacter(CachedEnemyCharacter.Get());
		}
	}

	UpdateHUDNamesFromDataAssets();
}

void AJujutsuKaisenPlayerController::UpdateHUDData()
{
	if (!PlayerHUDWidgetInstance)
	{
		return;
	}

	if (UPlayerHealthBarWidget* PlayerBar = PlayerHUDWidgetInstance->GetPlayerHealthBar())
	{
		if (CachedPlayerCharacter.IsValid())
		{
			PlayerBar->SetHealthValues(CachedPlayerCharacter->GetCurrentHealth(), CachedPlayerCharacter->GetMaxHealthValue());
		}
	}

	if (UEnemyHealthBarWidget* EnemyBar = PlayerHUDWidgetInstance->GetEnemyHealthBar())
	{
		if (CachedEnemyCharacter.IsValid())
		{
			EnemyBar->SetHealthValues(CachedEnemyCharacter->GetCurrentHealth(), CachedEnemyCharacter->GetMaxHealthValue());
		}
	}
}

void AJujutsuKaisenPlayerController::RefreshEnemyReference()
{
	if (!CachedPlayerCharacter.IsValid())
	{
		CachedPlayerCharacter = Cast<AJujutsuKaisenCharacter>(GetPawn());
	}

	if (!CachedEnemyCharacter.IsValid())
	{
		if (CachedPlayerCharacter.IsValid())
		{
			CachedEnemyCharacter = CachedPlayerCharacter->GetTargetCharacter();
		}
	}
	else
	{
		// Ensure target character updates (in case target changes)
		if (CachedPlayerCharacter.IsValid())
		{
			AJujutsuKaisenCharacter* CurrentTarget = CachedPlayerCharacter->GetTargetCharacter();
			if (CurrentTarget && CurrentTarget != CachedEnemyCharacter.Get())
			{
				CachedEnemyCharacter = CurrentTarget;

				if (PlayerHUDWidgetInstance)
				{
					if (UEnemyHealthBarWidget* EnemyBar = PlayerHUDWidgetInstance->GetEnemyHealthBar())
					{
						EnemyBar->InitializeWithCharacter(CurrentTarget);
					}
				}

				UpdateHUDNamesFromDataAssets();
			}
		}
	}

	if (PlayerHUDWidgetInstance && CachedEnemyCharacter.IsValid())
	{
		if (UEnemyHealthBarWidget* EnemyBar = PlayerHUDWidgetInstance->GetEnemyHealthBar())
		{
			if (EnemyBar->GetLinkedCharacter() == nullptr)
			{
				EnemyBar->InitializeWithCharacter(CachedEnemyCharacter.Get());
				UpdateHUDNamesFromDataAssets();
			}
		}
	}

	if (PlayerHUDWidgetInstance && CachedPlayerCharacter.IsValid())
	{
		if (UPlayerHealthBarWidget* PlayerBar = PlayerHUDWidgetInstance->GetPlayerHealthBar())
		{
			if (PlayerBar->GetLinkedCharacter() == nullptr)
			{
				PlayerBar->InitializeWithCharacter(CachedPlayerCharacter.Get());
				UpdateHUDNamesFromDataAssets();
			}
		}
	}
}

void AJujutsuKaisenPlayerController::UpdateHUDNamesFromDataAssets()
{
	if (!PlayerHUDWidgetInstance)
	{
		return;
	}

	if (UJujutsuKaisenGameInstance* JKGameInstance = Cast<UJujutsuKaisenGameInstance>(GetGameInstance()))
	{
		if (UJujutsuKaisenCharacterDataAsset* PlayerAsset = JKGameInstance->GetMyCharacterDataAsset())
		{
			const FString& PlayerName = PlayerAsset->GetCharacterName();
			if (!PlayerName.IsEmpty())
			{
				PlayerHUDWidgetInstance->SetPlayerDisplayName(FText::FromString(PlayerName));
			}
		}

		if (UJujutsuKaisenCharacterDataAsset* EnemyAsset = JKGameInstance->GetEnemyCharacterDataAsset())
		{
			const FString& EnemyName = EnemyAsset->GetCharacterName();
			if (!EnemyName.IsEmpty())
			{
				PlayerHUDWidgetInstance->SetEnemyDisplayName(FText::FromString(EnemyName));
			}
		}
	}
}
