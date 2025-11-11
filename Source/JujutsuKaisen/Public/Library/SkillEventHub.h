#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillEventHub.generated.h"

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

	// TODO: Camera Animation 지원은 추후 플러그인 활성화 이후 복구 예정
	// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraAnimationStart, UCameraAnimationSequence*, CameraAnim);
	// static FOnCameraAnimationStart OnCameraAnimationStartEvent;
};

