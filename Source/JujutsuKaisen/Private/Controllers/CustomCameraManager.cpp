#include "Controllers/CustomCameraManager.h"
#include "Library/SkillEventHub.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraShakeBase.h"
#include "CameraAnimationSequence.h"

ACustomCameraManager::ACustomCameraManager()
{
	bTargetOn = true;
}

void ACustomCameraManager::BeginPlay()
{
	Super::BeginPlay();

	USkillEventHub::OnCameraShakeStartEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraShakeStart);
	USkillEventHub::OnCameraShakeEndEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraShakeEnd);
	USkillEventHub::OnCameraAnimationStartEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraAnimationStart);
	USkillEventHub::OnCameraAnimationEndEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraAnimationEnd);
}

void ACustomCameraManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USkillEventHub::OnCameraShakeStartEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraShakeStart);
	USkillEventHub::OnCameraShakeEndEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraShakeEnd);
	USkillEventHub::OnCameraAnimationStartEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraAnimationStart);
	USkillEventHub::OnCameraAnimationEndEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraAnimationEnd);

	Super::EndPlay(EndPlayReason);
}

void ACustomCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (!bTargetOn)
	{
		return;
	}

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

void ACustomCameraManager::HandleCameraAnimationStart(UCameraAnimationSequence* CameraAnim)
{
	// UCameraAnimationSequence 재생을 위해 GameplayCameras 서브시스템 사용
	// 헤더 파일 경로 문제로 인해 블루프린트 호출 가능 함수로 구현
	if (CameraAnim && PCOwner)
	{
		// 일단 타겟 추적만 비활성화
		// 실제 카메라 애니메이션 재생은 블루프린트에서 구현하거나
		// GameplayCameras 플러그인이 활성화되어 있을 때 헤더 경로를 확인해야 함
		SetTargetOn(false);
	}
}

void ACustomCameraManager::HandleCameraAnimationEnd()
{
	SetTargetOn(true);
}

void ACustomCameraManager::SetTargetOn(bool bValue)
{
	bTargetOn = bValue;
}

