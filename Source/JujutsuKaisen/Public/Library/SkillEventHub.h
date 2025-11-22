#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillEventHub.generated.h"

class UCameraAnimationSequence;

/**
 * 스킬 관련 카메라 이벤트를 전파하는 공용 허브
 */
UCLASS()
class JUJUTSUKAISEN_API USkillEventHub : public UObject
{
	GENERATED_BODY()

public:
	// 카메라 쉐이크 시작
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraShakeStart);
	static FOnCameraShakeStart OnCameraShakeStartEvent;

	// 카메라 쉐이크 종료
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraShakeEnd);
	static FOnCameraShakeEnd OnCameraShakeEndEvent;

	// 카메라 애니메이션 시작
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraAnimationStart, UCameraAnimationSequence*, CameraAnim);
	static FOnCameraAnimationStart OnCameraAnimationStartEvent;

	// 카메라 애니메이션 종료
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraAnimationEnd);
	static FOnCameraAnimationEnd OnCameraAnimationEndEvent;
};

