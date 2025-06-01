// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GojoSatoruCharacter.h"

void AGojoSatoruCharacter::A_Skill(const FInputActionValue& Value)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,                   // Key (-1이면 계속 새로운 메시지로 표시)
            5.f,                  // Duration (화면에 몇 초 동안 표시할지)
            FColor::Red,          // 색상
            TEXT("Hello, world!") // 출력할 문자열
        );
    }
}