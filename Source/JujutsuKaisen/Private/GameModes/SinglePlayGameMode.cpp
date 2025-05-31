// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/SinglePlayGameMode.h"
#include "JujutsuKaisenGameInstance.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Animations/JujutsuKaisenAnimInstance.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"
#include "Controllers/JujutsuKaisenPlayerController.h"
#include "Controllers/JujutsuKaisenAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"


ASinglePlayGameMode::ASinglePlayGameMode()
{
    DefaultPawnClass = nullptr; // 자동 스폰 막기
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

    if (!GameInstance->GetMyCharacterDataAsset() || !GameInstance->GetEnemyCharacterDataAsset())
    {
        UE_LOG(LogTemp, Error, TEXT("Character Data asset is empty"));
        return;
    }


    SpawnCharacterFromData(GameInstance->GetMyCharacterDataAsset(), PlayerSpawnLocation, PlayerSpawnRotation, true);
    SpawnCharacterFromData(GameInstance->GetEnemyCharacterDataAsset(), EnemySpawnLocation, EnemySpawnRotation, false);

    PossessPlayer();
    PossessAI();
}

void ASinglePlayGameMode::SpawnCharacterFromData(UJujutsuKaisenCharacterDataAsset* DataAsset, const FVector& SpawnLocation, const FRotator& SpawnRotation, bool bIsPlayerCharacter)
{
    if (!DataAsset || !DataAsset->GetCharacterClass())
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

    // 캐릭터 스폰

    FTransform SpawnTransform(SpawnRotation, SpawnLocation);

    AJujutsuKaisenCharacter* SpawnedCharacter = Cast<AJujutsuKaisenCharacter>(
        UGameplayStatics::BeginDeferredActorSpawnFromClass(
            World,
            DataAsset->GetCharacterClass(),
            SpawnTransform,
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
            nullptr
        )
    );

    if (SpawnedCharacter)
    {
        // 메시, 데이터셋 등 초기화
        SpawnedCharacter->InitCharacterWithData(DataAsset);

        // 실제 BeginPlay() 호출됨
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
