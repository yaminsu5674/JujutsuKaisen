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

	// activate collision on Castser's HitBox;
	if (Caster)
	{
		Caster->ActivateAttack();
	}

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
	//		-1,                   // Key (-1�̸� ��� ���ο� �޽����� ǥ��)
	//		5.f,                  // Duration (ȭ�鿡 �� �� ���� ǥ������)
	//		FColor::Red,          // ����
	//		TEXT("Right Hook") // ����� ���ڿ�
	//	);
	//}

}
