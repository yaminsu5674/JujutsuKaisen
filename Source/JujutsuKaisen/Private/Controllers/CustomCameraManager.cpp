#include "Controllers/CustomCameraManager.h"
#include "Library/SkillEventHub.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Skills/SkillManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraShakeBase.h"
#include "CameraAnimationSequence.h"
#include "Animations/CameraAnimationCameraModifier.h"

ACustomCameraManager::ACustomCameraManager()
{
	bTargetOn = true;
}

void ACustomCameraManager::BeginPlay()
{
	Super::BeginPlay();

	// 제어하는 캐릭터의 SkillManager에 바인딩
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController)
	{
		APawn* ControlledPawn = PlayerController->GetPawn();
		AJujutsuKaisenCharacter* ControlledCharacter = Cast<AJujutsuKaisenCharacter>(ControlledPawn);
		
		if (ControlledCharacter && ControlledCharacter->GetSkillManager())
		{
			USkillManager* SkillManager = ControlledCharacter->GetSkillManager();
			
			// SkillManager의 델리게이트에 바인딩
			SkillManager->OnCameraShakeStartEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraShakeStart);
			SkillManager->OnCameraShakeEndEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraShakeEnd);
			SkillManager->OnCameraAnimationStartEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraAnimationStart);
			SkillManager->OnCameraAnimationEndEvent.AddDynamic(this, &ACustomCameraManager::HandleCameraAnimationEnd);
		}
	}
}

void ACustomCameraManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// SkillManager의 델리게이트에서 언바인딩
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController)
	{
		APawn* ControlledPawn = PlayerController->GetPawn();
		AJujutsuKaisenCharacter* ControlledCharacter = Cast<AJujutsuKaisenCharacter>(ControlledPawn);
		
		if (ControlledCharacter && ControlledCharacter->GetSkillManager())
		{
			USkillManager* SkillManager = ControlledCharacter->GetSkillManager();
			SkillManager->OnCameraShakeStartEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraShakeStart);
			SkillManager->OnCameraShakeEndEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraShakeEnd);
			SkillManager->OnCameraAnimationStartEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraAnimationStart);
			SkillManager->OnCameraAnimationEndEvent.RemoveDynamic(this, &ACustomCameraManager::HandleCameraAnimationEnd);
		}
	}

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
	if (!CameraAnim || !PCOwner)
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleCameraAnimationStart: PlayerController is null."));
		return;
	}

	// UCameraAnimationCameraModifier를 직접 사용 (ENGINECAMERAS_API 매크로가 있어서 정상 링크됨)
	UCameraAnimationCameraModifier* CameraModifier = UCameraAnimationCameraModifier::GetCameraAnimationCameraModifierFromPlayerController(PlayerController);
	if (!CameraModifier)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleCameraAnimationStart: CameraAnimationCameraModifier is null."));
		SetTargetOn(false); // 모디파이어가 없어도 타겟 추적은 비활성화
		return;
	}

	// 카메라 애니메이션 재생 (기본 파라미터 사용)
	FCameraAnimationParams Params;
	CameraModifier->PlayCameraAnimation(CameraAnim, Params);
	
	// 타겟 추적 비활성화
	SetTargetOn(false);
}

void ACustomCameraManager::HandleCameraAnimationEnd()
{
	if (!PCOwner)
	{
		SetTargetOn(true);
		return;
	}

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleCameraAnimationEnd: PlayerController is null."));
		SetTargetOn(true); // 컨트롤러가 없어도 타겟 추적은 재활성화
		return;
	}

	// UCameraAnimationCameraModifier를 직접 사용하여 모든 카메라 애니메이션 중지
	UCameraAnimationCameraModifier* CameraModifier = UCameraAnimationCameraModifier::GetCameraAnimationCameraModifierFromPlayerController(PlayerController);
	if (!CameraModifier)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleCameraAnimationEnd: CameraAnimationCameraModifier is null."));
		SetTargetOn(true); // 모디파이어가 없어도 타겟 추적은 재활성화
		return;
	}

	// 모든 카메라 애니메이션 중지
	CameraModifier->StopAllCameraAnimations(true);
	
	// 타겟 추적 재활성화
	SetTargetOn(true);
}

void ACustomCameraManager::SetTargetOn(bool bValue)
{
	bTargetOn = bValue;
}

