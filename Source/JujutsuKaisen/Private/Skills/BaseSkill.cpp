// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/BaseSkill.h"


void UBaseSkill::Activate(AJujutsuKaisenCharacter* Caster, AJujutsuKaisenCharacter* Target)
{
	if (!Caster)
	{
		UE_LOG(LogTemp, Warning, TEXT("FireballSkill: No caster!"));
		return;
	}
}

