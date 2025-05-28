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

void UJujutsuKaisenAnimInstance::SetState(ECharacterState InState)
{
    State = InState;
}


float UJujutsuKaisenAnimInstance::GetSpeed() const
{
    return Speed;
}

void UJujutsuKaisenAnimInstance::SetSpeed(float InSpeed)
{
    Speed = InSpeed;
}


ECharacterState UJujutsuKaisenAnimInstance::GetState() const
{
    return State;
}
