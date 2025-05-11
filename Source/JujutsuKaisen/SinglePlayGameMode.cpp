// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePlayGameMode.h"
#include "JujutsuKaisenGameInstance.h"
#include "JujutsuKaisenCharacter.h"
#include "JujutsuKaisenCharacterDataAsset.h"
#include "Kismet/GameplayStatics.h"

void ASinglePlayGameMode::BeginPlay()
{
    Super::BeginPlay();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Skill 1   !"));
    }

    UJujutsuKaisenGameInstance* GameInstance = Cast<UJujutsuKaisenGameInstance>(GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Log, TEXT("GameInstance Casting failed"));
        return;
    }

    if (!GameInstance->MyCharacterDataAsset || !GameInstance->EnemyCharacterDataAsset)
    {
        UE_LOG(LogTemp, Log, TEXT("Character Data asset is empty"));
        return;
    }

    FVector PlayerSpawnLocation(0.f, -200.f, 300.f);
    FVector EnemySpawnLocation(0.f, 200.f, 300.f);

    SpawnCharacterFromData(GameInstance->MyCharacterDataAsset, PlayerSpawnLocation, FRotator::ZeroRotator, true);
    SpawnCharacterFromData(GameInstance->EnemyCharacterDataAsset, EnemySpawnLocation, FRotator::ZeroRotator, false);
}

void ASinglePlayGameMode::SpawnCharacterFromData(UJujutsuKaisenCharacterDataAsset* DataAsset, const FVector& SpawnLocation, const FRotator& SpawnRotation, bool bIsPlayerCharacter)
{
    if (!DataAsset || !DataAsset->CharacterClass)
    {
        UE_LOG(LogTemp, Log, TEXT("Character Class or DataAsset is not valid"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Log, TEXT("No World"));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // 캐릭터 스폰
    AJujutsuKaisenCharacter* SpawnedCharacter = World->SpawnActor<AJujutsuKaisenCharacter>(DataAsset->CharacterClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (SpawnedCharacter)
    {
       

        if (bIsPlayerCharacter)
        {
            SpawnedCharacter->AutoPossessPlayer = EAutoReceiveInput::Player0;
            UE_LOG(LogTemp, Log, TEXT("Player Character %s Spawn Complete"), *SpawnedCharacter->GetName());
        }
        else
        {
            SpawnedCharacter->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
            UE_LOG(LogTemp, Log, TEXT("AI Character %s spawn complete"), *SpawnedCharacter->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Character spawn failed."));
    }
}
