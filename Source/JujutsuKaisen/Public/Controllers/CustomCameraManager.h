#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CustomCameraManager.generated.h"

class AJujutsuKaisenCharacter;

UCLASS()
class JUJUTSUKAISEN_API ACustomCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ACustomCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

protected:
	void UpdateCameraForCharacter(AJujutsuKaisenCharacter* ControlledCharacter, float DeltaTime);

	UPROPERTY()
	TWeakObjectPtr<AJujutsuKaisenCharacter> CachedCharacter;
};

