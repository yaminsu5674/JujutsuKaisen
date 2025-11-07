// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/JujutsuKaisenAnimInstance.h"
#include "Characters/JujutsuKaisenCharacter.h"


void UJujutsuKaisenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character) return;

    // 기존 애니메이션 변수들 업데이트
    Speed = Character->GetVelocity().Size();
    VerticalSpeed = Character->GetVelocity().Z;
    bIsFalling = VerticalSpeed < -1.f;



    // 상태 매니저에서 상태들 가져오기
    if (Character->GetStateManager())
    {
        ECharacterState PreviousState = CurrentState;
        CurrentState = Character->GetStateManager()->GetCurrentState();
        CurrentHitSubState = Character->GetStateManager()->GetCurrentHitSubState();
        bIsHitFront = Character->GetStateManager()->GetIsHitFront();

        // 상태 변화 감지 및 로그
        if (PreviousState != CurrentState)
        {
            UE_LOG(LogTemp, Warning, TEXT("State changed from %d to %d"), (int32)PreviousState, (int32)CurrentState);
        }
        
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
        UE_LOG(LogTemp, Warning, TEXT("OnStateAnimationEnds: Dead state, no transition"));
        return;
    }
    else
    {   
        // 다른 상태들은 로코모션으로 복귀
        if (Character && Character->GetStateManager())
        {
            ECharacterState PreviousState = CurrentState;
            Character->GetStateManager()->ForceState(ECharacterState::Locomotion);
          
            
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Character or StateManager is NULL in OnStateAnimationEnds"));
        }
        
        // 이동 가능하도록 설정
        if (Character)
        {
            Character->SetCanMove(true);
            // 스킬 변수들 리셋
            Character->ResetSkillVariables();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Character is NULL when trying to SetCanMove"));
        }
    }
}
