// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GojoSatoruCharacter.h"
#include "Skills/BaseSkill.h"
#include "Skills/Gojo_Satoru/Aka.h"
#include "Skills/Common/RightHook.h"

AGojoSatoruCharacter::AGojoSatoruCharacter()
{

}

void AGojoSatoruCharacter::BeginPlay()
{
    Super::BeginPlay();
    InitSkills();
}

void AGojoSatoruCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGojoSatoruCharacter::InitSkills()
{
    SkillManager->RegisterSkill("R", NewObject<UAka>(this));
    
    
    /*if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Skill Init!!!"));
    }*/
    
}
