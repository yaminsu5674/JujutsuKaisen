// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/SkillLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void USkillLibrary::RotateActorToFaceTarget(AActor* Source, AActor* Target)
{
	if (!Source || !Target) return;

	FVector Direction = (Target->GetActorLocation() - Source->GetActorLocation()).GetSafeNormal();
	//FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	//Source->SetActorRotation(NewRotation);
	FRotator LookAtRotation = Direction.Rotation();
	FRotator OnlyYawRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
	Source->SetActorRotation(OnlyYawRotation);
	
}

bool USkillLibrary::SimpleLineTrace(UObject* WorldContextObject, FVector Start, FVector End, FHitResult& OutHit)
{
	return true;
}

AJujutsuKaisenCharacter* USkillLibrary::GetOwnerRecursive(UObject* Object)
{
	if (!Object)
	{
		return nullptr;
	}
	
	// 이미 JujutsuKaisenCharacter인지 확인
	AJujutsuKaisenCharacter* Character = Cast<AJujutsuKaisenCharacter>(Object);
	if (Character)
	{
		return Character;
	}
	
	// GetOuter()로 타고 올라감
	UObject* Outer = Object->GetOuter();
	if (Outer)
	{
		// 재귀 호출
		return GetOwnerRecursive(Outer);
	}
	
	// 찾지 못함
	return nullptr;
}

bool USkillLibrary::JudgeHitFront(AActor* Attacker, AActor* Victim)
{
	if (!Attacker || !Victim)
	{
		return true; // 기본값 true 반환
	}

	// 공격자의 진행 방향 벡터 (XY만) - 기본적으로 Forward 벡터 사용
	FVector AttackerMovementDirection = Attacker->GetActorForwardVector();
	AttackerMovementDirection.Z = 0.0f;
	
	
	AttackerMovementDirection.Normalize();

	// 피격자가 앞으로 바라보고 있는 방향 벡터 (XY만)
	FVector VictimForward = Victim->GetActorForwardVector();
	VictimForward.Z = 0.0f;
	VictimForward.Normalize();

	// 내적 계산: 발사체 진행 방향과 피격자 Forward 벡터
	float DotProduct = FVector::DotProduct(AttackerMovementDirection, VictimForward);
	// 내적이 0 이하면 true (뒤에서 맞음), 그렇지 않으면 false (앞에서 맞음)
	return DotProduct < 0.0f;
}