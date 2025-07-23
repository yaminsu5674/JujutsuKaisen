// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillManager.h"

void USkillManager::RegisterOwner(AJujutsuKaisenCharacter* InOwner)
{
    Owner = InOwner;
}

void USkillManager::RegisterSkill(FName Key, UBaseSkill* Skill) 
{ 
    if (Owner)
    {
        Skill->SetOwner(Owner);
    }
    BoundSkills.Add(Key, Skill); 
}

void USkillManager::HandlePressed(FName Key, AJujutsuKaisenCharacter* Target)
{
    if (auto* Skill = BoundSkills.FindRef(Key))
    {
        Skill->OnPressed(Target);
    }
}

void USkillManager::HandleReleased(FName Key, AJujutsuKaisenCharacter* Target)
{
    if (auto* Skill = BoundSkills.FindRef(Key))
    {
        Skill->OnReleased(Target);
    }
}

void USkillManager::TickActiveSkills(float Delta)
{
    for (auto& Pair : BoundSkills)
    {
        if (Pair.Value && Pair.Value->GetBWantsTick())
        {
            Pair.Value->TickSkill(Delta);
        }
    }
}