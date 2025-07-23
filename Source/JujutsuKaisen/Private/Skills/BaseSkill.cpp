// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/BaseSkill.h"


void UBaseSkill::SetOwner(AJujutsuKaisenCharacter* NewOwner)
{
	Owner = NewOwner;
}


bool UBaseSkill::GetBWantsTick()
{
	return bWantsTick;
}

void UBaseSkill::TickSkill(float DeltaTime)
{

}


void UBaseSkill::Activate(AJujutsuKaisenCharacter* Caster, AJujutsuKaisenCharacter* Target)
{
	if (!Caster)
	{
		UE_LOG(LogTemp, Warning, TEXT("FireballSkill: No caster!"));
		return;
	}

	// if Target exists, rotation Caster's body to direction to Target
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
void UBaseSkill::OnPressed(AJujutsuKaisenCharacter* Target)
{

}

void UBaseSkill::OnReleased(AJujutsuKaisenCharacter* Target)
{

}

