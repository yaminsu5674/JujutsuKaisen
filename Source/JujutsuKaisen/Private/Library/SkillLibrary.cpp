// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/SkillLibrary.h"

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
