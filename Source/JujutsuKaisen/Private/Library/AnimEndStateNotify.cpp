// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/AnimEndStateNotify.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Animations/JujutsuKaisenAnimInstance.h"


void UAnimEndStateNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    AJujutsuKaisenCharacter* Character = Cast<AJujutsuKaisenCharacter>(Owner);
    if (!Character) return;

    Character->ForceState(ECharacterState::Locomotion);
}