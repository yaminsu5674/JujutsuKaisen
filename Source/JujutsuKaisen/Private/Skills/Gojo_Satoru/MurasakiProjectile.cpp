// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/MurasakiProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Library/SkillLibrary.h"
#include "Library/SkillEventHub.h"
#include "Characters/CharacterStateManager.h"

AMurasakiProjectile::AMurasakiProjectile()
{
	// HitSphereRadius 초기화
	HitSphereRadius = 100.0f;

	// 메시 컴포넌트 생성 (CollisionSphere에 부착)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionSphere); // Root인 CollisionSphere에 부착
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화 (시각적 용도만)
	
	// HitSphere 생성 및 설정 (Pawn과만 오버랩)
	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	HitSphere->SetupAttachment(CollisionSphere); // Root인 CollisionSphere에 부착
	HitSphere->InitSphereRadius(HitSphereRadius); // HitSphereRadius 멤버 변수 사용
	
	// HitSphere 충돌 설정 - Pawn과만 오버랩
	HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HitSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // 모든 채널 무시
	HitSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // Pawn만 오버랩
}

void AMurasakiProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HitSphere)
	{
		HitSphere->OnComponentBeginOverlap.AddDynamic(this, &AMurasakiProjectile::OnHitSphereOverlapBegin);
		HitSphere->OnComponentEndOverlap.AddDynamic(this, &AMurasakiProjectile::OnHitSphereOverlapEnd);
	}

	// 스폰 시 ChargingEffect 파티클 재생 (발사체에 붙어서 함께 움직임)
	if (ChargingEffect)
	{
		// RootComponent에 부착하여 발사체와 함께 움직임
		ChargingEffectComponent = UGameplayStatics::SpawnEmitterAttached(ChargingEffect, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		ChargingEffectComponent->SetAbsolute(false, false, false); // 위치/회전/스케일 전부 부모 따라감
		ChargingEffectComponent->SetWorldScale3D(GetActorScale3D());
	}

	// ProjectileMovement를 Rush 타입으로 설정
	if (ProjectileMovement)
	{
		ProjectileMovement->SetMoveType(EProjectileMoveType::Rush);
	}
}

void AMurasakiProjectile::OnProjectileOverlapBegin(AActor* OtherActor)
{
	
}

void AMurasakiProjectile::OnProjectileOverlapEnd(AActor* OtherActor)
{
	
}

void AMurasakiProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Murasaki 전용 Tick 로직
	if (bIsOverlapping && Target != nullptr)
	{
		// 발사체와 함께 이동하며 점진적으로 위로 띄우기
		if (ProjectileMovement)
		{
			// X, Y는 발사체와 동기화, Z는 점진적 상승 (한 번에 계산)
			FVector DeltaMovement = ProjectileMovement->Velocity * DeltaTime * 0.6f;
			DeltaMovement.Z = 50.f * DeltaTime;  // Z축은 초당 5 유닛씩 상승으로 덮어쓰기
			
			FVector NewLocation = Target->GetActorLocation() + DeltaMovement;
			Target->SetActorLocation(NewLocation, true);  // Sweep = false (충돌 무시하고 강제 이동)
			UGameplayStatics::ApplyDamage(
				Target,
				700.0f,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass());
		}
	}
	
}

void AMurasakiProjectile::Destroyed()
{
	
	
	// 부모 클래스의 Destroyed 호출 (중요!)
	Super::Destroyed();
}


void AMurasakiProjectile::OnHitSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Owner는 무시 (자신의 스킬에 맞지 않도록)
	if (OtherActor == GetOwner())
	{
		return;
	}

	// 발사체를 수평으로만 이동하도록 변경 (Z축 제거)
	if (ProjectileMovement)
	{
		FVector CurrentVelocity = ProjectileMovement->Velocity;
		CurrentVelocity.Z = 0.0f;  // Z축 속도 제거
		ProjectileMovement->Velocity = CurrentVelocity;
	}

	// HitCharacter (실제 충돌한 캐릭터)의 중력/이동 모드 설정 ⭐ 핵심!
	// JujutsuKaisenCharacter인지 확인
	AJujutsuKaisenCharacter* HitCharacter = Cast<AJujutsuKaisenCharacter>(OtherActor);
	if (HitCharacter && HitCharacter->GetCharacterMovement())
	{
		// 피격 하위 상태를 Stun으로 설정
		if (HitCharacter->GetStateManager() && GetOwner())
		{
			bool bIsHitFront = USkillLibrary::JudgeHitFront(GetOwner(), HitCharacter);
			HitCharacter->GetStateManager()->SetHitSubState(EHitSubState::Stun, bIsHitFront);
		}
	}
}

void AMurasakiProjectile::OnHitSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Owner는 무시 (자신의 스킬에 맞지 않도록)
	if (OtherActor == GetOwner())
	{
		return;
	}
	
	// JujutsuKaisenCharacter인지 확인
	AJujutsuKaisenCharacter* HitCharacter = Cast<AJujutsuKaisenCharacter>(OtherActor);
	if (HitCharacter)
	{
		USkillEventHub::OnCameraShakeEndEvent.Broadcast();
		// 피격 하위 상태를 KnockBack으로 설정
		if (HitCharacter->GetStateManager() && GetOwner())
		{
			bool bIsHitFront = USkillLibrary::JudgeHitFront(GetOwner(), HitCharacter);
			HitCharacter->GetStateManager()->SetHitSubState(EHitSubState::KnockBack, bIsHitFront);
		}
		
		// 발사체 속도 방향으로 캐릭터 날리기
		if (ProjectileMovement)
		{
			// 발사체 속도 방향 추출 (정규화)
			FVector LaunchDirection = ProjectileMovement->Velocity;
			LaunchDirection.Normalize();
			
			// XY 방향에만 300 적용, Z는 0
			FVector LaunchVelocity = FVector(
				LaunchDirection.X * 3000.f,  // X축 300
				LaunchDirection.Y * 3000.f,  // Y축 300
				1000.0f                         // Z축 0 (수평으로만)
			);
			
			HitCharacter->LaunchCharacter(LaunchVelocity, false, true);
			UGameplayStatics::ApplyDamage(
				HitCharacter,
				200.0f,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass());
		}
		
		// 발사체 자신을 즉시 파괴
		Destroy();
	}
}
