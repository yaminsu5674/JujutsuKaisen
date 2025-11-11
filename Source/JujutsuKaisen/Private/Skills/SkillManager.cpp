// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillManager.h"
#include "Characters/JujutsuKaisenCharacter.h"



void USkillManager::HandlePressed(FName Key)
{
    if (auto* Skill = BoundSkills.FindRef(Key))
    {
        Skill->OnPressed();
    }
}

void USkillManager::HandleReleased(FName Key)
{
    if (auto* Skill = BoundSkills.FindRef(Key))
    {
        Skill->OnReleased();
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

void USkillManager::ResetActiveSkills()
{
    for (auto& Pair : BoundSkills)
    {
        if (Pair.Value)
        {
            Pair.Value->ResetSkill();
        }
    }
}

TArray<FName> USkillManager::GetRegisteredSkillNames() const
{
    TArray<FName> SkillNames;
    for (const auto& Pair : BoundSkills)
    {
        if (Pair.Value)
        {
            SkillNames.Add(Pair.Key);
        }
    }
    return SkillNames;
}