// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/JujutsuKaisenAnimInstance.h"
#include "Characters/JujutsuKaisenCharacter.h"


void UJujutsuKaisenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character) return;

    Speed = Character->GetVelocity().Size();
    bIsDashing = Character->GetBIsDashing();
    float VerticalSpeed = Character->GetVelocity().Z;
    bIsFalling = VerticalSpeed < -1.f;
    JumpCount = Character->GetJumpCount();
    bDidSuperJump = Character->GetBDidSuperJump();
    bDidDoubleJump = Character->GetBDidDoubleJump();
    float PrevVerticalSpeed = 0.f;
    const bool bAboutToLand = !bIsFalling && VerticalSpeed <= -0.f;

    switch (Character->GetState())
    {
    case ECharacterState::Locomotion:
    {
        if (bAboutToLand)
        {
            State = EAnimState::Land;
        }
        else if (bIsFalling)
        {
            State = EAnimState::Falling;
        }
        else if (bDidDoubleJump)
        {
            // double Jump
            State = EAnimState::DoubleJump;
        }
        else if (bDidSuperJump)
        {
            // Super Jump
            State = EAnimState::SuperJump;
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
    





    // === DEBUG: 상태 표시 ===
    if (GEngine)
    {
        FString StateString;

        switch (State)
        {
        case EAnimState::Locomotion:   StateString = TEXT("Locomotion"); break;
        case EAnimState::Dash:         StateString = TEXT("Dash");       break;
        case EAnimState::Jump:         StateString = TEXT("Jump");       break;
        case EAnimState::DoubleJump:   StateString = TEXT("DoubleJump"); break;
        case EAnimState::SuperJump:    StateString = TEXT("SuperJump");  break;
        case EAnimState::Falling:      StateString = TEXT("Falling");    break;
        case EAnimState::Land:         StateString = TEXT("Land");       break;
        case EAnimState::Skill:        StateString = TEXT("Skill");      break;
        case EAnimState::Hit:          StateString = TEXT("Hit");        break;
        case EAnimState::Dead:         StateString = TEXT("Dead");       break;
        default:                       StateString = TEXT("Unknown");    break;
        }

        GEngine->AddOnScreenDebugMessage(
            /* Key */ -1,
            /* TimeToDisplay */ 0.f,
            /* Color */ FColor::Green,
            FString::Printf(TEXT("[AnimState] %s"), *StateString)
        );
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
