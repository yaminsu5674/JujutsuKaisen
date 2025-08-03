// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/JujutsuKaisenAnimInstance.h"
#include "Characters/JujutsuKaisenCharacter.h"


void UJujutsuKaisenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character) return;

    Speed = Character->GetVelocity().Size();
    bIsDashing = Character->GetBIsDashing();
    JumpCount = Character->GetJumpCount();

    switch (Character->GetState())
    {
    case ECharacterState::Locomotion:
    {
        if (JumpCount == 2)
        {
            // double Jump
        }
        else if (JumpCount == 1 && bIsDashing)
        {
            // Super Jump
        }
        else if (JumpCount == 1)
        {
            // Jump
            State = EAnimState::Jump;
        }
        else if (bIsDashing)
        {
            // Dashing
            State = EAnimState::Dash;
        }
        else
        {
            // Locomotion
            State = EAnimState::Locomotion;
        }
    }
    break;

    case ECharacterState::Skill:
        State = EAnimState::Skill;
        break;

    case ECharacterState::Hit:
        State = EAnimState::Hit;
        break;

    case ECharacterState::Dead:
        State = EAnimState::Dead;
        break;

    default:
        State = EAnimState::Locomotion;
        break;
    }
}

void UJujutsuKaisenAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    APawn* OwnerPawn = TryGetPawnOwner();
    if (OwnerPawn)
    {
        Character = Cast<AJujutsuKaisenCharacter>(OwnerPawn);
    }
}

void UJujutsuKaisenAnimInstance::OnStateAnimationEnds()
{
    if (State == EAnimState::Dead)
    {
        // do nothing
    }
    else
    {
        State = EAnimState::Locomotion;
    }
}
