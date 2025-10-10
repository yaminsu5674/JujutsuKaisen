// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack/Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 0.0f;
	_LifeCountingDown = Lifespan;
	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_MeshComponent);
	
	// 구체 충돌 컴포넌트 생성
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(_MeshComponent);
	CollisionSphere->SetSphereRadius(SphereRadius);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// ProjectileMovement 컴포넌트 생성
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(_MeshComponent);
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // 중력 비활성화
	ProjectileMovement->bAutoActivate = false; // 초기에는 비활성화
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);
	SetActorTickEnabled(true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//Reset();
	if (ProjectileMovement && _MeshComponent)
	{
		ProjectileMovement->SetUpdatedComponent(_MeshComponent);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Orange, FString::Printf(TEXT("Tick - UpdatedComponent: %s"), ProjectileMovement->UpdatedComponent ? TEXT("Valid") : TEXT("nullptr")));
		}
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_LifeCountingDown <= 0.0f)
		return;

	switch (BehaviorType)
	{
	case EProjectileBehaviorType::Move:
		HandleMovement(DeltaTime);
		break;
	case EProjectileBehaviorType::Place:
		// Place는 이동하지 않고 그 자리에 계속 존재
		break;
	default:
		break;
	}
}

void AProjectile::SetDirection(AJujutsuKaisenCharacter* InTarget)
{
	if (InTarget)
	{
		// 타겟의 Z축을 30 더한 위치로 발사
		FVector TargetLocation = InTarget->GetActorLocation();
		TargetLocation.Z += 30.0f;
		Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		SetActorRotation(Direction.Rotation());
	}
	else
	{
		Direction = GetActorForwardVector();
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("타겟 없음 - Direction: %s"), *Direction.ToString()));
		}
	}
	
	// ProjectileMovement에 방향과 속도 설정
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Direction * Speed;
		ProjectileMovement->InitialSpeed = Speed;
		ProjectileMovement->MaxSpeed = Speed;
	}
}

void AProjectile::SetTarget(AJujutsuKaisenCharacter* InTarget)
{
	Target = InTarget;
}

void AProjectile::SetBehaviorType(EProjectileBehaviorType NewType)
{
	BehaviorType = NewType;

	switch (BehaviorType)
	{
	case EProjectileBehaviorType::Move:
	{
		ProjectileMovement->bAutoActivate = true;
		SetLifeSpan(Lifespan);
		SetActorEnableCollision(true);
		
		// Velocity 재설정
		if (ProjectileMovement)
		{
			ProjectileMovement->Velocity = Direction * Speed;
			ProjectileMovement->InitialSpeed = Speed;
			ProjectileMovement->MaxSpeed = Speed;
		}
		
		// ProjectileMovement 활성화
		ProjectileMovement->Activate();
		
		// 오버랩 이벤트 바인딩
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
		break;
	}
	case EProjectileBehaviorType::Place:
	{
		// Place는 수명이 없고 스폰된 곳에서 계속 존재
		SetLifeSpan(0.0f);
		SetActorEnableCollision(true);
		// ProjectileMovement 비활성화 (움직이지 않음)
		ProjectileMovement->Deactivate();
		// 오버랩 이벤트 바인딩
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
		CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnOverlapEnd);
		break;
	}
	case EProjectileBehaviorType::None:
	default:
		SetActorEnableCollision(false);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("ProjectileMovement 비활성화"));
		}
		// ProjectileMovement 비활성화
		ProjectileMovement->Deactivate();
		break;
	}

}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 타겟 캐릭터 초기화
	Target = Cast<AJujutsuKaisenCharacter>(OtherActor);
}

void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 기본 오버랩 종료 처리 (자식 클래스에서 오버라이드 가능)
}

void AProjectile::HandleMovement(float DeltaTime)
{
	// ProjectileMovement가 활성화되어 있으면 자동으로 이동 처리
	// 수명만 관리하면 됨
	_LifeCountingDown -= DeltaTime;
	
	// 매 0.5초마다 상태 확인
	static float DebugTimer = 0.0f;
	DebugTimer += DeltaTime;
	
	if (DebugTimer >= 0.5f)
	{
		DebugTimer = 0.0f;
		
		if (GEngine && ProjectileMovement)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, FString::Printf(TEXT("Tick - 위치: %s"), *GetActorLocation().ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, FString::Printf(TEXT("Tick - Velocity: %s"), *ProjectileMovement->Velocity.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, FString::Printf(TEXT("Tick - IsActive: %s"), ProjectileMovement->IsActive() ? TEXT("true") : TEXT("false")));
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, FString::Printf(TEXT("Tick - UpdatedComponent: %s"), ProjectileMovement->UpdatedComponent ? TEXT("Valid") : TEXT("nullptr")));
		}
	}
}


void AProjectile::CheckOverlap()
{
	if (Target)
	{
		if (CollisionSphere->IsOverlappingActor(Target))
		{
			bIsOverlapping = true;
			if (Target->GetStateManager())
			{
				Target->GetStateManager()->SetHitSubState(EHitSubState::LightHit);
			}	
			UE_LOG(LogTemp, Error, TEXT("이미 겹쳐 있음: %s"), *Target->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PalProjectile: 타겟도 없노 ㅋ"));
	}
}