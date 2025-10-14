// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/AkaProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AAkaProjectile::AAkaProjectile()
{
	// 블루프린트에서 파티클 시스템을 할당할 예정이므로 생성자에서는 초기화하지 않음
	ShotEffectTimer = 0.0f;
	ShotEffectInterval = 0.2f; // 0.2초 간격
}

void AAkaProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 스폰 시 ChargingEffect 파티클 재생 (발사체에 붙어서 함께 움직임)
	if (ChargingEffect)
	{
		// RootComponent에 부착하여 발사체와 함께 움직임
		ChargingEffectComponent = UGameplayStatics::SpawnEmitterAttached(ChargingEffect, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	}
}

void AAkaProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부모의 OnOverlapBegin 호출 (Target 초기화)
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (Target != nullptr && !bIsOverlapping)
	{
		// ShotEffect 파티클 재생
		if (ShotEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, GetActorLocation(), GetActorRotation());
		}

		// 캐릭터에게 데미지 적용
		if (Target->GetStateManager())
		{
			Target->GetStateManager()->SetHitSubState(EHitSubState::MediumHit);
		}
		
		// 물리 충돌로 캐릭터 날리기
		if (Target->GetCharacterMovement())
        {
            // Projectile의 방향 벡터 (정규화된)
            // FVector LaunchDir = GetActorForwardVector();
            // FVector ImpulseForce = LaunchDir * 400.f + FVector(0, 0, 200.f);
            // Target->GetCharacterMovement()->AddImpulse(ImpulseForce, true);

        }
		bIsOverlapping = true;
		
		// ChargingEffect 제거
		if (ChargingEffectComponent)
		{
			ChargingEffectComponent->DestroyComponent();
			ChargingEffectComponent = nullptr;
		}
	}
}

void AAkaProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// ProjectileMovement가 충돌했는지 체크 (Velocity가 급격히 감소하면 충돌로 간주)
	// if (ProjectileMovement && BehaviorType == EProjectileBehaviorType::Move && !bIsOverlapping)
	// {
	// 	// Sweep으로 전방 충돌 체크
	// 	FHitResult HitResult;
	// 	FVector Start = GetActorLocation();
	// 	FVector End = Start + (Direction * 50.0f); // 50 유닛 앞 체크
		
	// 	FCollisionQueryParams QueryParams;
	// 	QueryParams.AddIgnoredActor(this);
	// 	if (GetOwner())
	// 	{
	// 		QueryParams.AddIgnoredActor(GetOwner());
	// 	}
		
	// 	// Sphere Sweep으로 충돌 체크
	// 	bool bHit = GetWorld()->SweepSingleByChannel(
	// 		HitResult,
	// 		Start,
	// 		End,
	// 		FQuat::Identity,
	// 		ECC_Pawn,
	// 		FCollisionShape::MakeSphere(CollisionSphere->GetScaledSphereRadius()),
	// 		QueryParams
	// 	);
		
	// 	// 충돌한 대상이 있으면 밀어내기
	// 	if (bHit && HitResult.GetActor())
	// 	{
	// 		AActor* HitActor = HitResult.GetActor();
	// 		AJujutsuKaisenCharacter* HitCharacter = Cast<AJujutsuKaisenCharacter>(HitActor);
			
	// 		if (HitCharacter && HitCharacter->GetCharacterMovement())
	// 		{
	// 			// 충돌 방향으로 강한 힘 적용
	// 			FVector PushDirection = Direction;
	// 			FVector ImpulseForce = PushDirection * 5000.0f + FVector(0, 0, 500.0f);
	// 			HitCharacter->GetCharacterMovement()->AddImpulse(ImpulseForce, true);

	// 		}
	// 	}
	// }
	
	// Aka 전용 Tick 로직
	if (bIsOverlapping && Target != nullptr)
	{
		// 오버랩 중일 때의 로직
		// 필요시 여기에 지속적인 데미지나 효과 추가 가능
		Target->Hit();
	}

	// Move 상태일 때 ShotEffect를 0.2초마다 생성
	if (BehaviorType == EProjectileBehaviorType::Move && ShotEffect && !bIsOverlapping)
	{
		ShotEffectTimer += DeltaTime;
		
		if (ShotEffectTimer >= ShotEffectInterval)
		{
			// 현재 위치에서 ShotEffect 파티클 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, GetActorLocation(), GetActorRotation());
			ShotEffectTimer = 0.0f; // 타이머 리셋
		}
	}
}
