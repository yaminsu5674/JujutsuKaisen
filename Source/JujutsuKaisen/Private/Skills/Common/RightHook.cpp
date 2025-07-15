// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Common/RightHook.h"

URightHook::URightHook()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(TEXT("AnimMontage'/Game/Static/Animation_Source/Right_Hook_Montage.Right_Hook_Montage'"));
	if (MontageFinder.Succeeded())
	{
		RightHookMontage = MontageFinder.Object;
	}
}

void URightHook::Activate(AJujutsuKaisenCharacter* Caster, AJujutsuKaisenCharacter* Target)
{
	Super::Activate(Caster, Target);

	

	// set HitBox's damage with damage member


	// play animMontage with INTERFFACE!!!!!!!!!!!!!!!!!!!!
	UAnimInstance* AnimInstance = Caster->GetMesh()->GetAnimInstance();
	if (AnimInstance && RightHookMontage)
	{
		AnimInstance->Montage_Play(RightHookMontage);
	}

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,                   // Key (-1이면 계속 새로운 메시지로 표시)
	//		5.f,                  // Duration (화면에 몇 초 동안 표시할지)
	//		FColor::Red,          // 색상
	//		TEXT("Right Hook") // 출력할 문자열
	//	);
	//}

}
