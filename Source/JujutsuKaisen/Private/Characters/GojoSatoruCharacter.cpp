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
    /*if (auto* Skill = SkillManager->BoundSkills.FindRef("R"))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Skill Init!!!"));
        }
    }*/
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Skill Init!!!"));
    }
    
}


//void AGojoSatoruCharacter::R_Pressed(const FInputActionValue& Value)
//{
//
//    // make Skill Object with INTERFACE!!!!!!!!!!!!!!!!!!!!!!!!!!
//    // 
//    // 
//    // use member function ex. activate(this);
//
//    UBaseSkill* Skill = NewObject<UAka>(this); // 'this'는 Outer
//
//    if (Skill)
//    {
//        Skill->Activate(this); // Target은 nullptr로 기본값
//    }
//
//
//}
//
//void AGojoSatoruCharacter::R_Released(const FInputActionValue& Value)
//{
//
//    if (GEngine)
//     {
//        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Released!!!"));
//     }
//
//
//}