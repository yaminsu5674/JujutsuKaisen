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
#include "Engine/BlueprintGeneratedClass.h"


ASinglePlayGameMode::ASinglePlayGameMode()
{
    DefaultPawnClass = nullptr; // 코드 직접 생성
    
    // PlayerControllerClass 강제 설정 (블루프린트 설정이 안될 경우)
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Dynamic/Blueprints/Controllers/BP_JujutsuKaisenPlayerController"));
    if (PlayerControllerBPClass.Succeeded())
    {
        PlayerControllerClass = PlayerControllerBPClass.Class;
    }
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

    if (PlayerCharacter && EnemyCharacter)
    {
        PlayerCharacter->SetTargetCharacter(EnemyCharacter);
        EnemyCharacter->SetTargetCharacter(PlayerCharacter);

        PlayerCharacter->InitSkills();
        EnemyCharacter->InitSkills();
    }
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
        UGameplayStatics::FinishSpawningActor(SpawnedCharacter, SpawnTransform);

        if (bIsPlayerCharacter)
        {
            PlayerCharacter = SpawnedCharacter;
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
    if (!World) 
    {
        UE_LOG(LogTemp, Error, TEXT("World is NULL"));
        return;
    }

    APlayerController* RawPC = UGameplayStatics::GetPlayerController(World, 0);
    AJujutsuKaisenPlayerController* PC = Cast<AJujutsuKaisenPlayerController>(RawPC);

    if (PC && PlayerCharacter)
    {
        PC->Possess(PlayerCharacter);
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
