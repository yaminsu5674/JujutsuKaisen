// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GojoSatoruCharacter.h"
#include "Skills/BaseSkill.h"
#include "Skills/Common/RightHook.h"

void AGojoSatoruCharacter::A_Skill(const FInputActionValue& Value)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,                   // Key (-1이면 계속 새로운 메시지로 표시)
            5.f,                  // Duration (화면에 몇 초 동안 표시할지)
            FColor::Red,          // 색상
            TEXT("A_Skill") // 출력할 문자열
        );
    }

    // make Skill Object with INTERFACE!!!!!!!!!!!!!!!!!!!!!!!!!!
    // 
    // 
    // use member function ex. activate(this);

    UBaseSkill* Skill = NewObject<URightHook>(this); // 'this'는 Outer

    if (Skill)
    {
        Skill->Activate(this); // Target은 nullptr로 기본값
    }


}