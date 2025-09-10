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

    // 애니메이션 인스턴스를 통해 OnStateAnimationEnds 호출
    UJujutsuKaisenAnimInstance* AnimInstance = Cast<UJujutsuKaisenAnimInstance>(MeshComp->GetAnimInstance());
    if (AnimInstance)
    {
        AnimInstance->OnStateAnimationEnds();
    }
    else
    {
        // 애니메이션 인스턴스가 없으면 에러 로그
        UE_LOG(LogTemp, Error, TEXT("AnimEndStateNotify: No AnimInstance found!"));
    }
}