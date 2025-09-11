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
        ECharacterState PreviousState = CurrentState;
        CurrentState = Character->GetStateManager()->GetCurrentState();
        CurrentHitSubState = Character->GetStateManager()->GetCurrentHitSubState();
        
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
        // 디버그 로그 추가
        UE_LOG(LogTemp, Warning, TEXT("OnStateAnimationEnds called. CurrentState: %d"), (int32)CurrentState);
        
        if (GEngine)
        {
            FString StateString = FString::Printf(TEXT("OnStateAnimationEnds - CurrentState: %d"), (int32)CurrentState);
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, StateString);
        }
        
        // 다른 상태들은 로코모션으로 복귀
        if (Character && Character->GetStateManager())
        {
            ECharacterState PreviousState = CurrentState;
            Character->GetStateManager()->ForceState(ECharacterState::Locomotion);
            
            // 상태 변경 확인 - 한 줄로 전후 비교
            ECharacterState NewState = Character->GetStateManager()->GetCurrentState();
            UE_LOG(LogTemp, Warning, TEXT("State transition: %d -> %d (Hit to Locomotion)"), (int32)PreviousState, (int32)NewState);
            
            if (GEngine)
            {
                FString TransitionString = FString::Printf(TEXT("State: %d -> %d"), (int32)PreviousState, (int32)NewState);
                GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TransitionString);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Character or StateManager is NULL in OnStateAnimationEnds"));
        }
        
        // 이동 가능하도록 설정
        if (Character)
        {
            Character->SetCanMove(true);
            UE_LOG(LogTemp, Warning, TEXT("SetCanMove(true) called"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Character is NULL when trying to SetCanMove"));
        }
    }
}
