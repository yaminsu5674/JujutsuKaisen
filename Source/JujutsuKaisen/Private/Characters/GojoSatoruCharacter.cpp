// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GojoSatoruCharacter.h"

void AGojoSatoruCharacter::A_Skill(const FInputActionValue& Value)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,                   // Key (-1�̸� ��� ���ο� �޽����� ǥ��)
            5.f,                  // Duration (ȭ�鿡 �� �� ���� ǥ������)
            FColor::Red,          // ����
            TEXT("Hello, world!") // ����� ���ڿ�
        );
    }
}