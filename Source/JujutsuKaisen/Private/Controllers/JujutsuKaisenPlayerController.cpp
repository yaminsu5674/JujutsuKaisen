// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/JujutsuKaisenPlayerController.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

AJujutsuKaisenPlayerController::AJujutsuKaisenPlayerController()
{
	// 기본 설정
}

void AJujutsuKaisenPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 캐릭터를 플레이어 모드로 설정
	if (AJujutsuKaisenCharacter* Char = Cast<AJujutsuKaisenCharacter>(GetPawn()))
	{
		Char->SetPlayerMode(true);
		UE_LOG(LogTemp, Log, TEXT("Player Character set to Player Mode"));
	}
}
