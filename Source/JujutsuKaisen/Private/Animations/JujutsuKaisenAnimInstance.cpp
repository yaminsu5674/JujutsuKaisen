// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/JujutsuKaisenAnimInstance.h"
#include "Characters/JujutsuKaisenCharacter.h"


void UJujutsuKaisenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwnerPawn = TryGetPawnOwner();
    if (!OwnerPawn) return;

    AJujutsuKaisenCharacter* Character = Cast<AJujutsuKaisenCharacter>(OwnerPawn);
    if (!Character) return;

    Speed = OwnerPawn->GetVelocity().Size();
    bIsDashing = Character->GetBIsDashing();
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
