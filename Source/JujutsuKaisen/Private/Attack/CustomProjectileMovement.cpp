// Fill out your copyright notice in the Description page of Project Settings.

#include "Attack/CustomProjectileMovement.h"

UCustomProjectileMovement::UCustomProjectileMovement()
{
	// 기본 설정
	bNeverStop = false;
	MoveType = EProjectileMoveType::Single; // 기본값: 단발형
	
	// 부모 클래스의 기본 설정 상속
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomProjectileMovement::BeginPlay()
{
	Super::BeginPlay();
	bNeverStop = true;
	ProjectileGravityScale = 0.f;
	// 초기화 로직
	if (bNeverStop)
	{
		// 충돌해도 멈추지 않도록 설정
		bShouldBounce = false;
	}
}

void UCustomProjectileMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// bNeverStop 모드일 때: 속도와 방향 저장
	FVector SavedVelocity = Velocity;
	FVector SavedDirection = Velocity.GetSafeNormal();
	float SavedSpeed = Velocity.Size();
	
	// 0.5초 간격으로 Super::TickComponent 호출
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
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, 
					FString::Printf(TEXT("Super Tick Called! DeltaTime: %.3f"), CapturedTime));
			}
		};
		
		// 람다 실행
		CallSuperTick();
		
		// 타이머 리셋
		AccumulatedTime = 0.0f;
	}
	
	// bNeverStop 모드: Super::TickComponent 이후 속도와 방향 강제 복원
	if (bNeverStop)
	{
		// 중력 완전 비활성화
		ProjectileGravityScale = 0.0f;
		
		// 속도와 방향 강제 복원 (중력이나 충돌로 변경된 것을 되돌림)
		if (SavedSpeed > 0.0f)
		{
			Velocity = SavedDirection * InitialSpeed;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, 
					FString::Printf(TEXT("Velocity Locked: %s"), *Velocity.ToString()));
			}
		}
	}
}

void UCustomProjectileMovement::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	// 충돌 디버그 메시지
	if (Hit.GetActor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("I got hit"));
		}
	}
	
	// 부모 클래스의 HandleImpact 호출 (기본 충돌 처리)
	//Super::HandleImpact(Hit, TimeSlice, MoveDelta);
}

UCustomProjectileMovement::EHandleBlockingHitResult UCustomProjectileMovement::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	// 충돌해도 멈추지 않고 계속 진행
	if (bNeverStop)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, TEXT("Blocking Hit Ignored - Continue!"));
		}
		// 방향 유지
        Velocity = GetOwner()->GetActorForwardVector() * Velocity.Size();
        ProjectileGravityScale = 0.f;
		
		// 충돌 무시하고 계속 진행
		return EHandleBlockingHitResult::AdvanceNextSubstep;
	}
	
	// 기본 동작 (멈춤)
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

