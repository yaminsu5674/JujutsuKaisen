// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/JujutsuKaisenAnimInstance.h"
#include "Characters/JujutsuKaisenCharacter.h"

void UJujutsuKaisenAnimInstance::OnStateAnimationEnds()
{
    if (State == ECharacterState::Dead)
    {
        // do nothing
    }
    else
    {
        State = ECharacterState::Locomotion;
    }
}
