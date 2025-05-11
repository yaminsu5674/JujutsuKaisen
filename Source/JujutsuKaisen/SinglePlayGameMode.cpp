// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePlayGameMode.h"
#include "JujutsuKaisenGameInstance.h"
#include "JujutsuKaisenCharacter.h"
#include "JujutsuKaisenCharacterDataAsset.h"
#include "JujutsuKaisenPlayerController.h"
#include "JujutsuKaisenAIController.h"
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

    if (!GameInstance->MyCharacterDataAsset || !GameInstance->EnemyCharacterDataAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("Character Data asset is empty"));
        return;
    }

    FVector PlayerSpawnLocation(0.f, -200.f, 0.f);
    FVector EnemySpawnLocation(0.f, 200.f, 0.f);

    SpawnCharacterFromData(GameInstance->MyCharacterDataAsset, PlayerSpawnLocation, FRotator::ZeroRotator, true);
    SpawnCharacterFromData(GameInstance->EnemyCharacterDataAsset, EnemySpawnLocation, FRotator::ZeroRotator, false);

    InitCharacterFromData(GameInstance->MyCharacterDataAsset, true);
    InitCharacterFromData(GameInstance->EnemyCharacterDataAsset, false);

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

    // 캐릭터 스폰
    AJujutsuKaisenCharacter* SpawnedCharacter = World->SpawnActor<AJujutsuKaisenCharacter>(DataAsset->CharacterClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (SpawnedCharacter)
    {
       

        if (bIsPlayerCharacter)
        {
            PlayerCharacter = SpawnedCharacter;
            /*SpawnedCharacter->AutoPossessPlayer = EAutoReceiveInput::Player0;*/
            UE_LOG(LogTemp, Log, TEXT("Player Character %s Spawn Complete"), *SpawnedCharacter->GetName());
        }
        else
        {
            EnemyCharacter = SpawnedCharacter;
            /*SpawnedCharacter->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;*/
            UE_LOG(LogTemp, Log, TEXT("AI Character %s spawn complete"), *SpawnedCharacter->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Character spawn failed."));
    }
}

void ASinglePlayGameMode::InitCharacterFromData(UJujutsuKaisenCharacterDataAsset* DataAsset, bool bIsPlayerCharacter)
{
    if (!DataAsset) return;

    AJujutsuKaisenCharacter* TargetCharacter = bIsPlayerCharacter ? PlayerCharacter : EnemyCharacter;

    if (!TargetCharacter || !TargetCharacter->GetMesh()) return;

    USkeletalMeshComponent* Mesh = TargetCharacter->GetMesh();

    // Skeletal Mesh 설정
    if (DataAsset->Mesh)
    {
        Mesh->SetSkeletalMesh(DataAsset->Mesh);
    }

    // AnimBP 설정
    if (DataAsset->AnimBP && DataAsset->AnimBP)
    {
        Mesh->SetAnimInstanceClass(DataAsset->AnimBP);
    }

    // Mesh 스케일 설정
    Mesh->SetRelativeScale3D(FVector(DataAsset->MeshScale));

    // Mesh 위치를 캡슐 아래로 내리기
    float HalfHeight = TargetCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    Mesh->SetRelativeLocation(FVector(0.f, 0.f, -HalfHeight));

    Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

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
