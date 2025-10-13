// Fill out your copyright notice in the Description page of Project Settings.

#include "Attack/CustomProjectileMovement.h"

UCustomProjectileMovement::UCustomProjectileMovement()
{
	// 기본 설정
	bNeverStop = false;
	
	// 부모 클래스의 기본 설정 상속
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomProjectileMovement::BeginPlay()
{
	Super::BeginPlay();
	
	// 초기화 로직
	if (bNeverStop)
	{
		// 충돌해도 멈추지 않도록 설정
		bShouldBounce = false;
	}
}

void UCustomProjectileMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// 커스텀 Tick 로직
	if (bNeverStop)
	{
		// 속도가 0이 되지 않도록 유지
		if (Velocity.IsNearlyZero())
		{
			Velocity = GetOwner()->GetActorForwardVector() * InitialSpeed;
		}
	}
}

