// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/MurasakiProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AMurasakiProjectile::AMurasakiProjectile()
{
	// 메시 컴포넌트 생성 (CollisionSphere에 부착)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionSphere); // Root인 CollisionSphere에 부착
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화 (시각적 용도만)
	
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

void AMurasakiProjectile::OnProjectileOverlapBegin(AActor* OtherActor)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("OnProjectileOverlapBegin Called!"));
	}
	
	// ShotEffect 파티클 재생 (기존 파티클 시스템 유지)
	if (ShotEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, GetActorLocation(), GetActorRotation());
		UE_LOG(LogTemp, Log, TEXT("MurasakiProjectile: Shot Particle Effect 시작"));
	}

	// 캐릭터에게 데미지 적용
	if (Target && Target->GetStateManager())
	{
		Target->GetStateManager()->SetHitSubState(EHitSubState::MediumHit);
	}
	
	// 물리 충돌로 캐릭터 날리기
	if (Target && Target->GetCharacterMovement())
	{
		// Projectile의 방향 벡터 (정규화된)
		FVector LaunchDir = GetActorForwardVector();
		FVector ImpulseForce = LaunchDir * 400.f + FVector(0, 0, 200.f);
		Target->GetCharacterMovement()->AddImpulse(ImpulseForce, true);
	}
	
	// ChargingEffect 제거
	if (ChargingEffectComponent)
	{
		ChargingEffectComponent->DestroyComponent();
		ChargingEffectComponent = nullptr;
	}
}

void AMurasakiProjectile::OnProjectileOverlapEnd(AActor* OtherActor)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("OnProjectileOverlapEnd Called!"));
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
}

