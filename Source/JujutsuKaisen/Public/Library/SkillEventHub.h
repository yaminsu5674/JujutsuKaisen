#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillEventHub.generated.h"

class UCameraAnimationSequence;

/**
 * 전역 시네마틱 애니메이션 이벤트를 전파하는 공용 허브
 * (카메라 쉐이크/애니메이션은 각 캐릭터의 SkillManager 인스턴스에 있음)
 */
UCLASS()
class JUJUTSUKAISEN_API USkillEventHub : public UObject
{
	GENERATED_BODY()

public:
	// 시네마틱 애니메이션 시작
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCinematicAnimationStart, UCameraAnimationSequence*, CameraAnim);
	static FOnCinematicAnimationStart OnCinematicAnimationStartEvent;

	// 시네마틱 애니메이션 종료
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCinematicAnimationEnd);
	static FOnCinematicAnimationEnd OnCinematicAnimationEndEvent;
};

