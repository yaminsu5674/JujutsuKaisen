#include "Library/CheckDeadNotify.h"

#include "Animation/AnimInstance.h"
#include "Characters/JujutsuKaisenCharacter.h"

void UCheckDeadNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AJujutsuKaisenCharacter* Character = Cast<AJujutsuKaisenCharacter>(MeshComp->GetOwner());
	if (!Character)
	{
		return;
	}

	if (!Character->GetIsDead())
	{
		return;
	}

	if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
	{
		if (UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage())
		{
			AnimInstance->Montage_Pause(CurrentMontage);
		}
		else
		{
			MeshComp->bPauseAnims = true;
		}
	}
}

