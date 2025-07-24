// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack/Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 0.0f;

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_MeshComponent);



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
		HandlePlacement(DeltaTime);
		break;
	default:
		break;
	}

	//_LifeCountingDown -= DeltaTime;
}

void AProjectile::InitializeParams(AJujutsuKaisenCharacter* InTarget)
{
	TargetCharacter = InTarget;

	_LifeCountingDown = Lifespan;

	if (TargetCharacter)
	{
		Direction = (TargetCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		SetActorRotation(Direction.Rotation()); 
	}
	else
	{
		Direction = GetActorForwardVector(); 
	}

	// auto destroy setting
	// SetLifeSpan(Lifespan);
}

void AProjectile::Reset()
{
	_LifeCountingDown = Lifespan;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void AProjectile::SetBehaviorType(EProjectileBehaviorType NewType)
{
	BehaviorType = NewType;
	if (BehaviorType == EProjectileBehaviorType::Move)
	{
		// auto destroy
		SetLifeSpan(Lifespan);
	}
}

void AProjectile::HandleMovement(float DeltaTime)
{
	FVector currentLocation = GetActorLocation();
	FVector nextLocation = currentLocation + Direction * Speed * DeltaTime;
	SetActorLocation(nextLocation);

	// Line trace to detect collision
	FHitResult hitResult;
	FCollisionObjectQueryParams objCollisionQueryParams;
	objCollisionQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);
	traceParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByObjectType(hitResult,
		currentLocation,
		nextLocation,
		objCollisionQueryParams))
	{
		auto Target = Cast<AJujutsuKaisenCharacter>(hitResult.GetActor());
		if (Target != nullptr)
		{
			//Target->Hit(Damage);
			PrimaryActorTick.bCanEverTick = false;
			Destroy();
		}
	}

	_LifeCountingDown -= DeltaTime;
}


void AProjectile::HandlePlacement(float DeltaTime)
{
	
}