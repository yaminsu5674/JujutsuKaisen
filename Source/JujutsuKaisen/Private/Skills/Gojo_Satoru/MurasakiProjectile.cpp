// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/MurasakiProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Library/SkillLibrary.h"
#include "Characters/CharacterStateManager.h"

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
	// 발사체를 수평으로만 이동하도록 변경 (Z축 제거)
	if (ProjectileMovement)
	{
		FVector CurrentVelocity = ProjectileMovement->Velocity;
		CurrentVelocity.Z = 0.0f;  // Z축 속도 제거
		ProjectileMovement->Velocity = CurrentVelocity;
	}

	AJujutsuKaisenCharacter* HitCharacter = Cast<AJujutsuKaisenCharacter>(OtherActor);
	if (HitCharacter && HitCharacter->GetCharacterMovement())
	{
		// 중력 끄기
		HitCharacter->GetCharacterMovement()->GravityScale = 0.0f;
		
	
		// 캐릭터 이동 비활성화 (CharacterMovement가 위치를 덮어쓰지 못하게)
		HitCharacter->GetCharacterMovement()->StopMovementImmediately();
		
		// 피격 하위 상태를 Stun으로 설정
		if (HitCharacter->GetStateManager() && GetOwner())
		{
			bool bIsHitFront = USkillLibrary::JudgeHitFront(GetOwner(), HitCharacter);
			HitCharacter->GetStateManager()->SetHitSubState(EHitSubState::Stun, bIsHitFront);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("HitCharacter Setup: GravityScale=0, MovementMode=Falling, StopMovement"));
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
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, 
					FString::Printf(TEXT("Launch: (%.1f, %.1f, %.1f)"), LaunchVelocity.X, LaunchVelocity.Y, LaunchVelocity.Z));
			}
		}
		
		// 발사체 자신을 즉시 파괴
		Destroy();
	}
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
		}
	}
	
}

void AMurasakiProjectile::Destroyed()
{
	
	
	// 부모 클래스의 Destroyed 호출 (중요!)
	Super::Destroyed();
}

