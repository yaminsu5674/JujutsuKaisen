// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/JujutsuKaisenAnimInstance.h"
#include "Characters/JujutsuKaisenCharacter.h"


void UJujutsuKaisenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character) return;

    // 기존 애니메이션 변수들 업데이트
    Speed = Character->GetVelocity().Size();
    float VerticalSpeed = Character->GetVelocity().Z;
    bIsFalling = VerticalSpeed < -1.f;



    // 상태 매니저에서 상태들 가져오기
    if (Character->GetStateManager())
    {
        CurrentState = Character->GetStateManager()->GetCurrentState();
        CurrentHitSubState = Character->GetStateManager()->GetCurrentHitSubState();
        
        // 물리 기반 Falling 상태 전환
        if (bIsFalling && CurrentState != ECharacterState::Falling)
        {
            Character->GetStateManager()->SetState(ECharacterState::Falling);
        }
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
    if (CurrentState == ECharacterState::Dead)
    {
        // 죽음 상태는 그대로 유지
        return;
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("OnStateAnimationEnds"));
        }
        // 다른 상태들은 로코모션으로 복귀
        if (Character && Character->GetStateManager())
        {
            Character->GetStateManager()->ForceState(ECharacterState::Locomotion);
        }
        
        // 이동 가능하도록 설정
        if (Character)
        {
            Character->SetCanMove(true);
        }
    }
}
