// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/AkaProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Attack/CustomProjectileMovement.h"

AAkaProjectile::AAkaProjectile()
{
	// 메시 컴포넌트 생성 (CollisionSphere에 부착)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionSphere); // Root인 CollisionSphere에 부착
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화 (시각적 용도만)
	
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
	
	// ProjectileMovement를 Rush 타입으로 설정
	if (ProjectileMovement)
	{
		ProjectileMovement->SetMoveType(EProjectileMoveType::Rush);
	}
}

void AAkaProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Owner와 오버랩되면 무시
	if (OtherActor == GetOwner())
	{
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("OnOverlapBegin Called!"));
	}
	
	// 부모의 OnOverlapBegin 호출 (bIsOverlapping 처리)
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	// 부모에서 bIsOverlapping이 true가 되었는지 확인
	if (bIsOverlapping)
	{
		// 발사체를 수평으로만 이동하도록 변경 (Z축 제거)
		if (ProjectileMovement)
		{
			FVector CurrentVelocity = ProjectileMovement->Velocity;
			CurrentVelocity.Z = 0.0f;  // Z축 속도 제거
			ProjectileMovement->Velocity = CurrentVelocity;
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
            // FVector LaunchDir = GetActorForwardVector();
            // FVector ImpulseForce = LaunchDir * 400.f + FVector(0, 0, 200.f);
            // Target->GetCharacterMovement()->AddImpulse(ImpulseForce, true);

        }
		
		// ChargingEffect 제거
		if (ChargingEffectComponent)
		{
			ChargingEffectComponent->DestroyComponent();
			ChargingEffectComponent = nullptr;
		}
	}
}

void AAkaProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 부모의 OnOverlapEnd 호출 (bIsOverlapping 처리)
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AAkaProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Aka 전용 Tick 로직
	if (bIsOverlapping && Target != nullptr)
	{
		// 오버랩 중일 때의 로직
		//Target->Hit();
		
		// 발사체의 현재 속도로 캐릭터 밀어내기 (발사체와 함께 이동)
		if (ProjectileMovement)
		{
			FVector LaunchVelocity = ProjectileMovement->Velocity;
			Target->LaunchCharacter(LaunchVelocity, true, true);
		}
	}
	// 움직이고 있을 때 (Velocity > 0) ShotEffect를 0.2초마다 생성
	if (ProjectileMovement && ProjectileMovement->Velocity.Size() > 0.0f && ShotEffect)
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
