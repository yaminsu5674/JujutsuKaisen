// Fill out your copyright notice in the Description page of Project Settings.


#include "JujutsuKaisenAnimInstance.h"
#include "JujutsuKaisenCharacter.h"

void UJujutsuKaisenAnimInstance::OnStateAnimationEnds()
{
    if (State == ECharacterState::Dead)
    {
        // do nothing
    }
    else
    {
        State = ECharacterState::Idle;
    }
}
