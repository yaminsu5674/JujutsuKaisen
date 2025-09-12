// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Sukuna/PalProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

APalProjectile::APalProjectile()
{
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

void APalProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부모의 OnOverlapBegin 호출 (Target 초기화)
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (Target != nullptr && !bIsOverlapping)
	{
		// ShotEffect 파티클 재생

		bIsOverlapping = true;
	}
    if (Target->GetStateManager())
    {
        Target->GetStateManager()->SetHitSubState(EHitSubState::LightHit);
    }
}

void APalProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 부모의 OnOverlapEnd 호출
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	// 오버랩이 끝나면 bIsOverlapping을 false로 설정
	if (bIsOverlapping)
	{
		bIsOverlapping = false;
	}
}

void APalProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Pal 전용 Tick 로직
	if (bIsOverlapping && Target != nullptr)
	{
		// 오버랩 중일 때의 로직
		// 필요시 여기에 지속적인 데미지나 효과 추가 가능
		Target->Hit();
	}

	// Move 상태일 때 ShotEffect 파티클 생성
	if (BehaviorType == EProjectileBehaviorType::Move && ShotEffect && !bIsOverlapping)
	{
		// 이동 중 파티클 이펙트 (선택적)
		// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, GetActorLocation(), GetActorRotation());
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
            FVector ImpulseForce = LaunchDir * 400.f + FVector(0, 0, 200.f);
            Target->GetCharacterMovement()->AddImpulse(ImpulseForce, true);

        }
    }
    Destroy();
}
