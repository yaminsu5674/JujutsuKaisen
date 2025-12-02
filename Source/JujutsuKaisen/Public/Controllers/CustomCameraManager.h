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

	// 카메라 쉐이크 시작 (인스턴스 멤버 - 각 캐릭터별로 독립적)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraShakeStart);
	UPROPERTY()
	FOnCameraShakeStart OnCameraShakeStartEvent;

	// 카메라 쉐이크 종료
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraShakeEnd);
	UPROPERTY()
	FOnCameraShakeEnd OnCameraShakeEndEvent;

	// 카메라 애니메이션 시작
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraAnimationStart, UCameraAnimationSequence*, CameraAnim);
	UPROPERTY()
	FOnCameraAnimationStart OnCameraAnimationStartEvent;

	// 카메라 애니메이션 종료
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraAnimationEnd);
	UPROPERTY()
	FOnCameraAnimationEnd OnCameraAnimationEndEvent;

	// 캐릭터로부터 CustomCameraManager를 얻는 헬퍼 함수
	static ACustomCameraManager* GetCustomCameraManagerFromCharacter(AJujutsuKaisenCharacter* Character);

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

	bool bTargetOn;

	UPROPERTY(EditAnywhere, Category = "Camera|Shake")
	TSubclassOf<UCameraShakeBase> DefaultCameraShake;
};

