// Fill out your copyright notice in the Description page of Project Settings.


#include "JujutsuKaisenGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/SinglePlayGameMode.h"
#include "GameModes/LobbyGameMode.h"



void UJujutsuKaisenGameInstance::StartSingle()
{
    auto world = GEngine->GetCurrentPlayWorld();
    if (world)
    {
        AWorldSettings* WorldSettings = world->GetWorldSettings();
        if (WorldSettings)
        {
            // URL 파라미터 없이 오픈하여 월드 세팅의 게임 모드 사용
            UGameplayStatics::OpenLevel(world, "CityMap", true);
            
        }
    }

}

void UJujutsuKaisenGameInstance::LeaveSingle()
{

    auto world = GEngine->GetCurrentPlayWorld();
    if (world)
    {
        AWorldSettings* WorldSettings = world->GetWorldSettings();
        if (WorldSettings)
        {
            UGameplayStatics::OpenLevel(world, "LobbyMap", true);
        }
    }
}


void UJujutsuKaisenGameInstance::Init()
{
    Super::Init();
    MyCharacterDataAsset = nullptr;
    EnemyCharacterDataAsset = nullptr;
}


void UJujutsuKaisenGameInstance::SetMyCharacterByPath(const FString& AssetPath)
{
    FSoftObjectPath DataAssetPath(AssetPath);
    UJujutsuKaisenCharacterDataAsset* LoadedAsset = Cast<UJujutsuKaisenCharacterDataAsset>(DataAssetPath.TryLoad());

    if (LoadedAsset)
    {
        MyCharacterDataAsset = LoadedAsset;
        UE_LOG(LogTemp, Log, TEXT("MyCharacterDataAsset set to %s"), *LoadedAsset->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load MyCharacterDataAsset from path: %s"), *AssetPath);
    }
}

void UJujutsuKaisenGameInstance::SetEnemyCharacterByPath(const FString& AssetPath)
{
    FSoftObjectPath DataAssetPath(AssetPath);
    UJujutsuKaisenCharacterDataAsset* LoadedAsset = Cast<UJujutsuKaisenCharacterDataAsset>(DataAssetPath.TryLoad());

    if (LoadedAsset)
    {
        EnemyCharacterDataAsset = LoadedAsset;
        UE_LOG(LogTemp, Log, TEXT("EnemyCharacterDataAsset set to %s"), *LoadedAsset->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load EnemyCharacterDataAsset from path: %s"), *AssetPath);
    }
}