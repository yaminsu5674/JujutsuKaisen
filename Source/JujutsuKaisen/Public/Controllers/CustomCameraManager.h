#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CustomCameraManager.generated.h"

class AJujutsuKaisenCharacter;
class UCameraAnimationSequence;
class UCameraShakeBase;

UCLASS()
class JUJUTSUKAISEN_API ACustomCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ACustomCameraManager();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

protected:
	UFUNCTION()
	void HandleCameraShakeStart();

	UFUNCTION()
	void HandleCameraShakeEnd();

	// TODO: 카메라 애니메이션 기능 재도입 시 복구
	// UFUNCTION()
	// void HandleCameraAnimationStart(UCameraAnimationSequence* CameraAnim);

	void UpdateCameraForCharacter(AJujutsuKaisenCharacter* ControlledCharacter, float DeltaTime);

	UPROPERTY()
	TWeakObjectPtr<AJujutsuKaisenCharacter> CachedCharacter;

	UPROPERTY(EditAnywhere, Category = "Camera|Shake")
	TSubclassOf<UCameraShakeBase> DefaultCameraShake;
};

