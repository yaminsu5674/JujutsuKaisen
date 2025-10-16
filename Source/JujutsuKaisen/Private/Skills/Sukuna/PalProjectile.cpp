// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Sukuna/PalProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/JujutsuKaisenCharacter.h"

APalProjectile::APalProjectile()
{
	// 메시 컴포넌트 생성 (CollisionSphere에 부착)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionSphere); // Root인 CollisionSphere에 부착
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화 (시각적 용도만)
	
	// 나이아가라 컴포넌트 생성 (차징 이펙트용)
	ChargingNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargingNiagaraComponent"));
	ChargingNiagaraComponent->SetupAttachment(RootComponent);
	ChargingNiagaraComponent->SetAutoActivate(false);
}

void APalProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 스폰 시 ChargingNiagaraEffect 재생
	if (ChargingNiagaraEffect && ChargingNiagaraComponent)
	{
		ChargingNiagaraComponent->SetAsset(ChargingNiagaraEffect);
		ChargingNiagaraComponent->Activate();
		UE_LOG(LogTemp, Log, TEXT("PalProjectile: Charging Niagara Effect 시작"));
	}

	
}

void APalProjectile::OnProjectileOverlapBegin(AActor* OtherActor)
{
	UE_LOG(LogTemp, Log, TEXT("PalProjectile: OnProjectileOverlapBegin 호출됨!"));
	UE_LOG(LogTemp, Log, TEXT("PalProjectile: 오버랩 처리 성공!"));
	
	if (Target && Target->GetStateManager())
	{
		Target->GetStateManager()->SetHitSubState(EHitSubState::LightHit);
		UE_LOG(LogTemp, Log, TEXT("PalProjectile: LightHit 설정 완료!"));
	}
}

void APalProjectile::OnProjectileOverlapEnd(AActor* OtherActor)
{
	// 기본 구현 (필요시 커스텀 로직 추가)
}

void APalProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Pal 전용 Tick 로직
	if (bIsOverlapping && Target != nullptr)
	{
		Target->Hit();
	}

}

void APalProjectile::EndPal()
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, GetActorLocation(), GetActorRotation());
	// ChargingNiagaraEffect 비활성화
	if (ChargingNiagaraComponent)
	{
		ChargingNiagaraComponent->Deactivate();
	}
    if (bIsOverlapping)
    {
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
            FVector ImpulseForce = LaunchDir * 2000.f + FVector(0, 0, 600.f);
            Target->GetCharacterMovement()->AddImpulse(ImpulseForce, true);

        }
    }
    Destroy();
}

