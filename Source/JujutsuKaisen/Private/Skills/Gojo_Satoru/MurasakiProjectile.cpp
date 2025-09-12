// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/MurasakiProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AMurasakiProjectile::AMurasakiProjectile()
{
	// 블루프린트에서 파티클 시스템을 할당할 예정이므로 생성자에서는 초기화하지 않음
}

void AMurasakiProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 스폰 시 ChargingEffect 파티클 재생 (발사체에 붙어서 함께 움직임)
	if (ChargingEffect)
	{
		// RootComponent에 부착하여 발사체와 함께 움직임
		ChargingEffectComponent = UGameplayStatics::SpawnEmitterAttached(ChargingEffect, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		ChargingEffectComponent->SetAbsolute(false, false, false); // 위치/회전/스케일 전부 부모 따라감
		ChargingEffectComponent->SetWorldScale3D(GetActorScale3D());
	}
}

void AMurasakiProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부모의 OnOverlapBegin 호출 (Target 초기화)
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (Target != nullptr && !bIsOverlapping)
	{
		// ShotEffect 파티클 재생 (기존 파티클 시스템 유지)
		if (ShotEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, GetActorLocation(), GetActorRotation());
			UE_LOG(LogTemp, Log, TEXT("MurasakiProjectile: Shot Particle Effect 시작"));
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
            FVector LaunchDir = GetActorForwardVector();
            FVector ImpulseForce = LaunchDir * 400.f + FVector(0, 0, 200.f);
            Target->GetCharacterMovement()->AddImpulse(ImpulseForce, true);

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

void AMurasakiProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Murasaki 전용 Tick 로직
	if (bIsOverlapping && Target != nullptr)
	{
		// 오버랩 중일 때의 로직
		// 필요시 여기에 지속적인 데미지나 효과 추가 가능
		Target->Hit();
	}

	// Move 상태일 때 ShotEffect 파티클 생성 (기존 파티클 시스템 유지)
	if (BehaviorType == EProjectileBehaviorType::Move && ShotEffect && !bIsOverlapping)
	{
		// 현재 위치에서 ShotEffect 파티클 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, GetActorLocation(), GetActorRotation());
	}
}

