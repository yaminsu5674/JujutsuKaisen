// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillManager.h"
#include "Characters/JujutsuKaisenCharacter.h"

void USkillManager::RegisterOwner(AJujutsuKaisenCharacter* InOwner)
{
    Owner = InOwner;
}

void USkillManager::InitAllSkillOwner() 
{ 
    if (Owner)
    {
        for (auto& SkillPair : BoundSkills)
        {
            UBaseSkill* Skill = SkillPair.Value;
            if (Skill)
            {
                Skill->SetOwner(Owner);
                if (Owner->GetTargetCharacter())
                {
                    Skill->SetTarget(Owner->GetTargetCharacter());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Owner has no target when InitAllSkillOwner is called for: %s"), *SkillPair.Key.ToString());
                }
            }
        }
    }
}

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