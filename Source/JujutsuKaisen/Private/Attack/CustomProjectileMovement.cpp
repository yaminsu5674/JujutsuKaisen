// Fill out your copyright notice in the Description page of Project Settings.

#include "Attack/CustomProjectileMovement.h"

UCustomProjectileMovement::UCustomProjectileMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomProjectileMovement::BeginPlay()
{
	Super::BeginPlay();
	MoveType = EProjectileMoveType::Single;

}

void UCustomProjectileMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	switch (MoveType)
	{
	case EProjectileMoveType::Single:
	case EProjectileMoveType::Rush:
		// 단발형/돌진형: 부모 클래스 로직 그대로
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		break;
		
	case EProjectileMoveType::Pulse:
	{
		// Pulse 타입: 이동/정지 반복
		static float PulseTimer = 0.0f;
		PulseTimer += DeltaTime;

		// 이동 지속 시간 (PulseInterval의 절반)
		const float MoveDuration = PulseInterval * 0.5f;

		if (PulseTimer < MoveDuration)
		{
			// 짧은 시간 동안 이동
			Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		}
		else if (PulseTimer >= PulseInterval)
		{
			PulseTimer = 0.0f; // 주기 리셋
		}
		// else: 정지 상태 (MoveDuration ~ PulseInterval 사이)

		break;
	}
	}
}

void UCustomProjectileMovement::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	switch (MoveType)
	{
	case EProjectileMoveType::Single:
	case EProjectileMoveType::Rush:
		// 단발형/돌진형: 부모 클래스 로직 그대로 (충돌 시 멈춤)
		Super::HandleImpact(Hit, TimeSlice, MoveDelta);
		break;
		
	case EProjectileMoveType::Pulse:
		// 펄스형: 충돌해도 멈추지 않음
		// Super::HandleImpact 호출 안 함 → 멈추지 않음
		break;
	}
}

UCustomProjectileMovement::EHandleBlockingHitResult UCustomProjectileMovement::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	switch (MoveType)
	{
	case EProjectileMoveType::Single:
	case EProjectileMoveType::Rush:
		// 단발형/돌진형: 부모 클래스 로직 그대로 (충돌 시 멈춤)
		return Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
		
	case EProjectileMoveType::Pulse:
		// 펄스형: 충돌해도 속도 유지하며 계속 진행
		{
			// 방향과 속도 크기 유지
			if (GetOwner())
			{
				float CurrentSpeed = Velocity.Size();
				Velocity = GetOwner()->GetActorForwardVector() * CurrentSpeed;
				ProjectileGravityScale = 0.0f;
			}
			
			// 계속 진행
			return EHandleBlockingHitResult::AdvanceNextSubstep;
		}
	}
	
	// 기본 동작
	return Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
}

void UCustomProjectileMovement::SetMoveType(EProjectileMoveType NewMoveType)
{
	MoveType = NewMoveType;
	
	// MoveType에 따라 설정 변경
	switch (MoveType)
	{
	case EProjectileMoveType::Single:
	case EProjectileMoveType::Rush:
		// 단발형/돌진형: 기본 설정
		bNeverStop = false;
		bShouldBounce = false;
		bIsHomingProjectile = false;
		break;
		
	case EProjectileMoveType::Pulse:
		// Pulse 타입: 멈추지 않음
		bNeverStop = true;
		bShouldBounce = false;
		bIsHomingProjectile = false;
		break;
	}
}

void UCustomProjectileMovement::SetPulseInterval(float NewInterval)
{
	PulseInterval = NewInterval;
}

void UCustomProjectileMovement::SetDirection(AActor* TargetActor, float ProjectileSpeed)
{
	if (!GetOwner()) return;
	
	FVector Direction;
	
	if (TargetActor)
	{
		// 타겟의 Z축을 30 더한 위치로 발사
		FVector TargetLocation = TargetActor->GetActorLocation();
		TargetLocation.Z += 30.0f;
		Direction = (TargetLocation - GetOwner()->GetActorLocation()).GetSafeNormal();
		GetOwner()->SetActorRotation(Direction.Rotation());
	}
	else
	{
		Direction = GetOwner()->GetActorForwardVector();
	}
	
	// Velocity와 속도 설정
	Velocity = Direction * ProjectileSpeed;
	InitialSpeed = ProjectileSpeed;
	MaxSpeed = ProjectileSpeed;
}

void UCustomProjectileMovement::ApplyBehaviorSettings(bool bShouldMove, bool bEnableLifespan, float LifespanDuration)
{
	if (bShouldMove)
	{
		// Move 타입: 정상적으로 이동
		// Velocity는 이미 SetDirection에서 설정됨
	}
	else
	{
		// Place/None 타입: 정지
		Velocity = FVector::ZeroVector;
	}
	
	// Lifespan은 Actor 레벨에서 관리하므로 여기서는 Movement 관련 설정만
}

