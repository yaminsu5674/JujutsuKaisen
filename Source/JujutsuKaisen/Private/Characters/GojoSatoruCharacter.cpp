// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GojoSatoruCharacter.h"
#include "Skills/BaseSkill.h"
#include "Skills/Gojo_Satoru/Aka.h"
#include "Skills/Common/RightHook.h"

void AGojoSatoruCharacter::A_Skill(const FInputActionValue& Value)
{

    // make Skill Object with INTERFACE!!!!!!!!!!!!!!!!!!!!!!!!!!
    // 
    // 
    // use member function ex. activate(this);

    UBaseSkill* Skill = NewObject<URightHook>(this); // 'this'�� Outer
    
    if (Skill)
    {
        Skill->Activate(this); // Target�� nullptr�� �⺻��
    }


}

void AGojoSatoruCharacter::R_Skill(const FInputActionValue& Value)
{

    // make Skill Object with INTERFACE!!!!!!!!!!!!!!!!!!!!!!!!!!
    // 
    // 
    // use member function ex. activate(this);

    UBaseSkill* Skill = NewObject<UAka>(this); // 'this'�� Outer

    if (Skill)
    {
        Skill->Activate(this); // Target�� nullptr�� �⺻��
    }


}