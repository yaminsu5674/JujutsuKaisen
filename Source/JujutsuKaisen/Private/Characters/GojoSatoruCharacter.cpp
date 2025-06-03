// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GojoSatoruCharacter.h"
#include "Skills/BaseSkill.h"
#include "Skills/Common/RightHook.h"

void AGojoSatoruCharacter::A_Skill(const FInputActionValue& Value)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,                   // Key (-1�̸� ��� ���ο� �޽����� ǥ��)
            5.f,                  // Duration (ȭ�鿡 �� �� ���� ǥ������)
            FColor::Red,          // ����
            TEXT("A_Skill") // ����� ���ڿ�
        );
    }

    // make Skill Object with INTERFACE!!!!!!!!!!!!!!!!!!!!!!!!!!
    // 
    // 
    // use member function ex. activate(this);

    UBaseSkill* Skill = NewObject<URightHook>(this); // 'this'�� Outer

    if (Skill)
    {
        Skill->Activate(this); // Target�� nullptr�� �⺻��
    }


}