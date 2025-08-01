// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/BaseSkill.h"
#include "Characters/JujutsuKaisenCharacter.h"


void UBaseSkill::SetOwner(AJujutsuKaisenCharacter* NewOwner)
{
	Owner = NewOwner;
	if (Owner)
	{
		AnimInstance = Owner->GetMesh()->GetAnimInstance();
	}
}

void UBaseSkill::SetTarget(AJujutsuKaisenCharacter* NewTarget)
{
	Target = NewTarget;
	/*if (Owner)
	{
		AnimInstance = Owner->GetMesh()->GetAnimInstance();
	}*/
}

bool UBaseSkill::GetBWantsTick()
{
	return bWantsTick;
}

void UBaseSkill::TickSkill(float DeltaTime)
{

}


void UBaseSkill::OnPressed()
{

}

void UBaseSkill::OnReleased()
{

}

