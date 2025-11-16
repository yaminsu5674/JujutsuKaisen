#include "Controllers/CustomCameraManager.h"
#include "Controllers/JujutsuKaisenPlayerController.h"
#include "Library/SkillEventHub.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraShakeBase.h"

ACustomCameraManager::ACustomCameraManager()
{
}

void ACustomCameraManager::BeginPlay()
{
	Super::BeginPlay();

	USkillEventHub::OnCameraShakeStartEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraShakeStart);
	USkillEventHub::OnCameraShakeEndEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraShakeEnd);
}

void ACustomCameraManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USkillEventHub::OnCameraShakeStartEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraShakeStart);
	USkillEventHub::OnCameraShakeEndEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraShakeEnd);

	Super::EndPlay(EndPlayReason);
}

void ACustomCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (CachedCharacter.IsValid())
	{
		UpdateCameraForCharacter(CachedCharacter.Get(), DeltaTime);
		return;
	}

	if (!PCOwner)
	{
		return;
	}

	APawn* ControlledPawn = PCOwner->GetPawn();
	AJujutsuKaisenCharacter* ControlledCharacter = Cast<AJujutsuKaisenCharacter>(ControlledPawn);

	if (!ControlledCharacter)
	{
		CachedCharacter.Reset();
		return;
	}

	CachedCharacter = ControlledCharacter;
	UpdateCameraForCharacter(ControlledCharacter, DeltaTime);
}

void ACustomCameraManager::UpdateCameraForCharacter(AJujutsuKaisenCharacter* ControlledCharacter, float DeltaTime)
{
	if (!ControlledCharacter)
	{
		return;
	}

	USpringArmComponent* CameraBoom = ControlledCharacter->GetCameraBoom();
	AJujutsuKaisenCharacter* TargetCharacter = ControlledCharacter->GetTargetCharacter();

	if (!CameraBoom || !TargetCharacter)
	{
		return;
	}

	const FVector OriginLocation = CameraBoom->GetComponentLocation();
	const FVector TargetLocation = TargetCharacter->GetActorLocation();

	FRotator DesiredRotation = (TargetLocation - OriginLocation).Rotation();
	DesiredRotation.Yaw -= 10.0f;

	const FRotator CurrentRotation = CameraBoom->GetComponentRotation();
	const FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, 2.0f);

	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetWorldRotation(InterpolatedRotation);
}

void ACustomCameraManager::HandleCameraShakeStart()
{
	if (DefaultCameraShake)
	{
		StartCameraShake(DefaultCameraShake);
	}
}

void ACustomCameraManager::HandleCameraShakeEnd()
{
	StopAllCameraShakes(true);
}

