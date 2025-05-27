// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GojoSatoruCharacter.h"

void AGojoSatoruCharacter::Skill_1()
{
	 if (GEngine)
	 {
	 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Skill 1   !"));	
	 }
}

void AGojoSatoruCharacter::Skill_2()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Skill 2   !"));
	}
}

void AGojoSatoruCharacter::Skill_3()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Skill 3   !"));
	}
}

