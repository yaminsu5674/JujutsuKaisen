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
		// 단발형: 부모 클래스 로직 그대로
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		break;
		
	case EProjectileMoveType::Rush:
	{
		// 돌진형: 속도와 방향 저장
		FVector SavedDirection = Velocity.GetSafeNormal();
		float SavedSpeed = Velocity.Size();
		
		// 0.2초 간격으로 Super::TickComponent 호출
		static float AccumulatedTime = 0.0f;
		AccumulatedTime += DeltaTime;
		
		if (AccumulatedTime >= 0.2f)
		{
			// 지역 변수로 복사
			float CapturedTime = AccumulatedTime;
			
			// 람다로 Super::TickComponent 호출
			auto CallSuperTick = [this, CapturedTime, TickType, ThisTickFunction]()
			{
				// 누적된 시간으로 한 번에 Tick
				Super::TickComponent(CapturedTime, TickType, ThisTickFunction);
			};
			
			// 람다 실행
			CallSuperTick();
			
			// 타이머 리셋
			AccumulatedTime = 0.0f;
		}
		
		// Super::TickComponent 이후 속도와 방향 강제 복원
		ProjectileGravityScale = 0.0f;
		
		if (SavedSpeed > 0.0f)
		{
			Velocity = SavedDirection * InitialSpeed;
		}
		break;
	}
	}
}

void UCustomProjectileMovement::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	switch (MoveType)
	{
	case EProjectileMoveType::Single:
		// 단발형: 부모 클래스 로직 그대로 (충돌 시 멈춤)
		Super::HandleImpact(Hit, TimeSlice, MoveDelta);
		break;
		
	case EProjectileMoveType::Rush:
		// 돌진형: 충돌 디버그만 출력, 멈추지 않음
		if (Hit.GetActor())
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("I got hit"));
			}
		}
		// Super::HandleImpact 호출 안 함 → 멈추지 않음
		break;
	}
}

UCustomProjectileMovement::EHandleBlockingHitResult UCustomProjectileMovement::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	switch (MoveType)
	{
	case EProjectileMoveType::Single:
		// 단발형: 부모 클래스 로직 그대로 (충돌 시 멈춤)
		return Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
		
	case EProjectileMoveType::Rush:
		// 돌진형: 충돌해도 계속 진행
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, TEXT("Blocking Hit Ignored - Continue!"));
		}
		
		// 방향과 속도 유지
		if (GetOwner())
		{
			Velocity = GetOwner()->GetActorForwardVector() * Velocity.Size();
			ProjectileGravityScale = 0.0f;
		}
		
		// 충돌 무시하고 계속 진행
		return EHandleBlockingHitResult::AdvanceNextSubstep;
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
		// 단발형: 기본 설정
		bNeverStop = false;
		bShouldBounce = false;
		bIsHomingProjectile = false;
		break;
		
	case EProjectileMoveType::Rush:
		// 돌진형: 멈추지 않음
		bNeverStop = true;
		bShouldBounce = false;
		bIsHomingProjectile = false;
		break;
	}
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

