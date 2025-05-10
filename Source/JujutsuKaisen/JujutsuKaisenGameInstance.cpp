// Fill out your copyright notice in the Description page of Project Settings.


#include "JujutsuKaisenGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SinglePlayGameMode.h"
#include "LobbyGameMode.h"



void UJujutsuKaisenGameInstance::StartSingle()
{
    auto world = GEngine->GetCurrentPlayWorld();
    if (world)
    {
        AWorldSettings* WorldSettings = world->GetWorldSettings();
        if (WorldSettings)
        {

            WorldSettings->DefaultGameMode = ASinglePlayGameMode::StaticClass();
            UGameplayStatics::OpenLevel(world, "CityMap", true);
        }
    }
}

void UJujutsuKaisenGameInstance::LeaveGame()
{

    auto world = GEngine->GetCurrentPlayWorld();
    if (world)
    {
        AWorldSettings* WorldSettings = world->GetWorldSettings();
        if (WorldSettings)
        {

            WorldSettings->DefaultGameMode = ALobbyGameMode::StaticClass();
            UGameplayStatics::OpenLevel(world, "LobbyMap", true);
        }
    }
}


void UJujutsuKaisenGameInstance::Init()
{
    Super::Init();
}

