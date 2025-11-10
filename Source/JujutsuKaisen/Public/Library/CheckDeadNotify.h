#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CheckDeadNotify.generated.h"

/**
 * Anim notify that checks if the owning character is dead and stops any playing montages if so.
 */
UCLASS()
class JUJUTSUKAISEN_API UCheckDeadNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

