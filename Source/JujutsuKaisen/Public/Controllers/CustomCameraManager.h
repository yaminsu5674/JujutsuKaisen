#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CustomCameraManager.generated.h"

class AJujutsuKaisenCharacter;
class UCameraAnimationSequence;
class UCameraShakeBase;
class UCameraAnimationCameraModifier;

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

	UFUNCTION()
	void HandleCameraAnimationStart(UCameraAnimationSequence* CameraAnim);

	UFUNCTION()
	void HandleCameraAnimationEnd();

	void UpdateCameraForCharacter(AJujutsuKaisenCharacter* ControlledCharacter, float DeltaTime);

	void SetTargetOn(bool bValue);

	UPROPERTY()
	TWeakObjectPtr<AJujutsuKaisenCharacter> CachedCharacter;

	UPROPERTY()
	bool bTargetOn;

	UPROPERTY(EditAnywhere, Category = "Camera|Shake")
	TSubclassOf<UCameraShakeBase> DefaultCameraShake;
};

