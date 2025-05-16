// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePlayGameMode.h"
#include "JujutsuKaisenGameInstance.h"
#include "JujutsuKaisenCharacter.h"
#include "JujutsuKaisenAnimInstance.h"
#include "JujutsuKaisenCharacterDataAsset.h"
#include "JujutsuKaisenPlayerController.h"
#include "JujutsuKaisenAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"


ASinglePlayGameMode::ASinglePlayGameMode()
{
    DefaultPawnClass = nullptr; // �ڵ� ���� ����
    PlayerControllerClass = AJujutsuKaisenPlayerController::StaticClass();
}


void ASinglePlayGameMode::BeginPlay()
{
    Super::BeginPlay();

    UJujutsuKaisenGameInstance* GameInstance = Cast<UJujutsuKaisenGameInstance>(GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance Casting failed"));
        return;
    }

    if (!GameInstance->MyCharacterDataAsset || !GameInstance->EnemyCharacterDataAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("Character Data asset is empty"));
        return;
    }

    FVector PlayerSpawnLocation(0.f, -200.f, 0.f);
    FVector EnemySpawnLocation(0.f, 200.f, 0.f);

    SpawnCharacterFromData(GameInstance->MyCharacterDataAsset, PlayerSpawnLocation, FRotator::ZeroRotator, true);
    SpawnCharacterFromData(GameInstance->EnemyCharacterDataAsset, EnemySpawnLocation, FRotator::ZeroRotator, false);

    /*InitCharacterFromData(GameInstance->MyCharacterDataAsset, true);
    InitCharacterFromData(GameInstance->EnemyCharacterDataAsset, false);*/

    PossessPlayer();
    PossessAI();
}

void ASinglePlayGameMode::SpawnCharacterFromData(UJujutsuKaisenCharacterDataAsset* DataAsset, const FVector& SpawnLocation, const FRotator& SpawnRotation, bool bIsPlayerCharacter)
{
    if (!DataAsset || !DataAsset->CharacterClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Character Class or DataAsset is not valid"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("No World"));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // ĳ���� ����

    // Legacy code ��� ���������� ���Ž� ����.
    // AJujutsuKaisenCharacter* SpawnedCharacter = World->SpawnActor<AJujutsuKaisenCharacter>(DataAsset->CharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
    FTransform SpawnTransform(SpawnRotation, SpawnLocation);

    AJujutsuKaisenCharacter* SpawnedCharacter = Cast<AJujutsuKaisenCharacter>(
        UGameplayStatics::BeginDeferredActorSpawnFromClass(
            World,
            DataAsset->CharacterClass,
            SpawnTransform,
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
            nullptr
        )
    );

    if (SpawnedCharacter)
    {
        // �޽�, �����ͼ� �� �ʱ�ȭ
        SpawnedCharacter->InitCharacterWithData(DataAsset);

        // ���� BeginPlay() ȣ���
        UGameplayStatics::FinishSpawningActor(SpawnedCharacter, SpawnTransform);

        if (bIsPlayerCharacter)
        {
            PlayerCharacter = SpawnedCharacter;
            UE_LOG(LogTemp, Log, TEXT("Player Character %s Spawn Complete"), *SpawnedCharacter->GetName());
        }
        else
        {
            EnemyCharacter = SpawnedCharacter;

            UE_LOG(LogTemp, Log, TEXT("AI Character %s spawn complete"), *SpawnedCharacter->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Character spawn failed."));
    }
}


void ASinglePlayGameMode::PossessPlayer()
{
    UWorld* World = GetWorld();

    if (!World) return;

    AJujutsuKaisenPlayerController* PC = Cast<AJujutsuKaisenPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
    if (PC)
    {
        PC->Possess(PlayerCharacter);
        UE_LOG(LogTemp, Log, TEXT("Player Character Possessed"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController not found"));
    }
}

void ASinglePlayGameMode::PossessAI()
{
    if (!EnemyCharacter) return;

    AJujutsuKaisenAIController* AIController = Cast<AJujutsuKaisenAIController>(EnemyCharacter->GetController());

    if (!AIController)
    {
        UWorld* World = GetWorld();
        if (!World) return;

        AIController = World->SpawnActor<AJujutsuKaisenAIController>(AJujutsuKaisenAIController::StaticClass());
        if (AIController)
        {
            AIController->Possess(EnemyCharacter);
            UE_LOG(LogTemp, Log, TEXT("AI Possession Successful"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AIController spawn failed"));
        }
    }
}
