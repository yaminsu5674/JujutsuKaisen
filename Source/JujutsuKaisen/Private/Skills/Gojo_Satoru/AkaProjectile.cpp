// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/AkaProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Attack/CustomProjectileMovement.h"
#include "Components/SphereComponent.h"
#include "Library/SkillLibrary.h"
#include "Library/SkillEventHub.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Characters/CharacterStateManager.h"

AAkaProjectile::AAkaProjectile()
{
	// 메시 컴포넌트 생성 (CollisionSphere에 부착)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionSphere); // Root인 CollisionSphere에 부착
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화 (시각적 용도만)
	
	// HitSphere 생성 및 설정 (Pawn과만 오버랩)
	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	HitSphere->SetupAttachment(CollisionSphere); // Root인 CollisionSphere에 부착
	HitSphere->InitSphereRadius(100.0f); // 반지름 50 (CollisionSphere와 비슷하게)
	
	// HitSphere 충돌 설정 - Pawn과만 오버랩
	HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HitSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // 모든 채널 무시
	HitSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // Pawn만 오버랩
	
	// 블루프린트에서 파티클 시스템을 할당할 예정이므로 생성자에서는 초기화하지 않음
	ShotEffectTimer = 0.0f;
	ShotEffectInterval = 0.2f; // 0.2초 간격
}

void AAkaProjectile::BeginPlay()
{
	Super::BeginPlay();  // 부모에서 SetLifeSpan(Lifespan) 자동 호출됨

	// HitSphere 오버랩 이벤트 바인딩
	if (HitSphere)
	{
		HitSphere->OnComponentBeginOverlap.AddDynamic(this, &AAkaProjectile::OnHitSphereOverlapBegin);
		HitSphere->OnComponentEndOverlap.AddDynamic(this, &AAkaProjectile::OnHitSphereOverlapEnd);
	}

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

void AAkaProjectile::OnProjectileOverlapBegin(AActor* OtherActor)
{
	
}

void AAkaProjectile::OnProjectileOverlapEnd(AActor* OtherActor)
{

}

void AAkaProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Aka 전용 Tick 로직 (HitSphere 오버랩 중일 때만)
	if (bIsOverlapping && Target != nullptr)
	{
		// 발사체와 함께 이동하며 점진적으로 위로 띄우기
		if (ProjectileMovement)
		{

			Target->TakeDamage(5.0f);
			// X, Y는 발사체와 동기화, Z는 점진적 상승 (한 번에 계산)
			FVector DeltaMovement = ProjectileMovement->Velocity * DeltaTime * 1.0f;
			DeltaMovement.Z = 50.f * DeltaTime;  // Z축은 초당 5 유닛씩 상승으로 덮어쓰기
			
			FVector NewLocation = Target->GetActorLocation() + DeltaMovement;
			Target->SetActorLocation(NewLocation, true);  // Sweep = false (충돌 무시하고 강제 이동)
		}
	}
	// 움직이고 있을 때 (Velocity > 0) ShotEffect를 ShotEffectInterval초마다 생성
	if (ProjectileMovement && ProjectileMovement->Velocity.Size() > 0.0f && ShotEffect)
	{
		ShotEffectTimer += DeltaTime;
		
		if (ShotEffectTimer >= ShotEffectInterval)
		{
			// 현재 위치에서 ShotEffect 파티클 생성 (스케일 3배)
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), 
				ShotEffect, 
				GetActorLocation(), 
				GetActorRotation(),
				FVector(1.0f, 1.0f, 1.0f)  // 크기 3배로 확대
			);
			ShotEffectTimer = 0.0f; // 타이머 리셋
		}
	}
}

void AAkaProjectile::Destroyed()
{
	// 부모 클래스의 Destroyed 호출 (중요!)

	Super::Destroyed();
}

void AAkaProjectile::OnHitSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	// ChargingEffect 제거
	if (ChargingEffectComponent)
	{
		ChargingEffectComponent->DestroyComponent();
		ChargingEffectComponent = nullptr;
	}

	USkillEventHub::OnCameraShakeStartEvent.Broadcast();
}

void AAkaProjectile::OnHitSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
			HitCharacter->TakeDamage(200.0f);
		}
		
		// 발사체 자신을 즉시 파괴
		Destroy();
	}

	USkillEventHub::OnCameraShakeEndEvent.Broadcast();
}
