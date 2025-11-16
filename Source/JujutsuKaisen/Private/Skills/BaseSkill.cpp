// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/BaseSkill.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Library/SkillLibrary.h"
#include "Library/SkillEventHub.h"

UBaseSkill::UBaseSkill()
{
	bWantsTick = false;
}

AJujutsuKaisenCharacter* UBaseSkill::GetOwner() const
{
	return USkillLibrary::GetOwnerRecursive(const_cast<UBaseSkill*>(this));
}

AJujutsuKaisenCharacter* UBaseSkill::GetTarget() const
{
	AJujutsuKaisenCharacter* Owner = GetOwner();
	return Owner ? Owner->GetTargetCharacter() : nullptr;
}

UAnimInstance* UBaseSkill::GetAnimInstance() const
{
	AJujutsuKaisenCharacter* Owner = GetOwner();
	return Owner ? Owner->GetMesh()->GetAnimInstance() : nullptr;
}

bool UBaseSkill::GetBWantsTick()
{
	return bWantsTick;
}

void UBaseSkill::TickSkill(float DeltaTime)
{

}

void UBaseSkill::ResetSkill()
{

}


void UBaseSkill::OnPressed()
{

}

void UBaseSkill::OnReleased()
{

}

