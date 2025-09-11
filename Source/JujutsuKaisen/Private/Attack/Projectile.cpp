// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack/Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);
	SetActorTickEnabled(true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	Reset();
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
		Direction = (InTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		SetActorRotation(Direction.Rotation()); 
	}
	else
	{
		Direction = GetActorForwardVector(); 
	}
}


void AProjectile::SetBehaviorType(EProjectileBehaviorType NewType)
{
	BehaviorType = NewType;

	switch (BehaviorType)
	{
	case EProjectileBehaviorType::Move:
	{
		SetLifeSpan(Lifespan);
		SetActorEnableCollision(true);
		// 오버랩 이벤트 바인딩
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
		break;
	}
	case EProjectileBehaviorType::Place:
	{
		// Place는 수명이 없고 스폰된 곳에서 계속 존재
		SetLifeSpan(0.0f);
		SetActorEnableCollision(true);
		// 오버랩 이벤트 바인딩
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
		break;
	}
	case EProjectileBehaviorType::None:
	default:
		SetActorEnableCollision(false);
		break;
	}
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩 상태를 true로 설정
	bIsOverlapping = true;
	
	// 타겟 캐릭터 초기화
	Target = Cast<AJujutsuKaisenCharacter>(OtherActor);
}

void AProjectile::HandleMovement(float DeltaTime)
{
	FVector currentLocation = GetActorLocation();
	FVector nextLocation = currentLocation + Direction * Speed * DeltaTime;
	SetActorLocation(nextLocation);

	_LifeCountingDown -= DeltaTime;
}
