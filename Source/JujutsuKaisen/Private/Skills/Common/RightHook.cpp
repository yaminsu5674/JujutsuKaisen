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

//void URightHook::Activate(AJujutsuKaisenCharacter* Caster, AJujutsuKaisenCharacter* Target)
//{
//	Super::Activate(Caster, Target);
//
//	
//
//	// set HitBox's damage with damage member
//
//
//	// play animMontage with INTERFFACE!!!!!!!!!!!!!!!!!!!!
//	UAnimInstance* AnimInstance = Caster->GetMesh()->GetAnimInstance();
//	if (AnimInstance && RightHookMontage)
//	{
//		AnimInstance->Montage_Play(RightHookMontage);
//	}
//
//
//}
