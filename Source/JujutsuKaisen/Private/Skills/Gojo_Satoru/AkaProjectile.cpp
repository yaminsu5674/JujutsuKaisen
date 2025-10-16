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
	Super::BeginPlay();  // 부모에서 SetLifeSpan(Lifespan) 자동 호출됨

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

	// 캐릭터에게 데미지 적용
	// if (Target && Target->GetStateManager())
	// {
	// 	Target->GetStateManager()->SetHitSubState(EHitSubState::MediumHit);
	// }
	
	// 물리 충돌로 캐릭터 날리기
	if (Target && Target->GetCharacterMovement())
	{
		// 중력 끄고 Falling 모드로 전환 (공중에 떠있을 수 있게)
		Target->GetCharacterMovement()->GravityScale = 0.0f;
		//Target->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		//Target->SetActorLocation(Target->GetActorLocation() + FVector(0, 0, 30.f));
	}
	
	// ChargingEffect 제거
	if (ChargingEffectComponent)
	{
		ChargingEffectComponent->DestroyComponent();
		ChargingEffectComponent = nullptr;
	}
}

void AAkaProjectile::OnProjectileOverlapEnd(AActor* OtherActor)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("OnProjectileOverlapEnd Called!"));
	}
	
	// 오버랩이 끝날 때 타겟을 날림
	if (Target && Target->GetCharacterMovement())
	{
		// 중력 다시 켜기 (프로젝트 기본값 2.8)
		Target->GetCharacterMovement()->GravityScale = 2.8f;
		
		// 발사체 속도 방향으로 캐릭터 날리기
		if (ProjectileMovement)
		{
			// 발사체 방향만 추출 (정규화)
			FVector LaunchDirection = ProjectileMovement->Velocity;
			LaunchDirection.Normalize();
			
			// 각 축에 하드코딩된 힘 적용
			FVector LaunchVelocity = FVector(
				LaunchDirection.X * 100.f,  // X축 100
				LaunchDirection.Y * 100.f,  // Y축 100
				100.f                        // Z축 100 (위로)
			);
			
			Target->LaunchCharacter(LaunchVelocity, true, true);
		}
	}
}

void AAkaProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Aka 전용 Tick 로직
	if (bIsOverlapping && Target != nullptr)
	{
		// 오버랩 중일 때의 로직
		//Target->Hit();
		
		// 발사체와 함께 이동하며 점진적으로 위로 띄우기
		// if (ProjectileMovement)
		// {
		// 	// 발사체의 이동량 계산 (X, Y, Z) 
		// 	FVector DeltaMovement = ProjectileMovement->Velocity * DeltaTime * 1.0f;
			
		// 	// Z축에 추가 상승량 누적 (매 프레임마다 조금씩 더 위로)
		// 	DeltaMovement.Z += 50.f * DeltaTime;  // 초당 50 유닛씩 추가 상승
			
		// 	// 타겟을 발사체와 같은 양 + 추가 상승량만큼 이동 (Sweep = false로 충돌 무시)
		// 	FVector NewLocation = Target->GetActorLocation() + DeltaMovement;
		// 	Target->SetActorLocation(NewLocation, false);  // false = 충돌 체크 없이 강제 이동
		// }
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
