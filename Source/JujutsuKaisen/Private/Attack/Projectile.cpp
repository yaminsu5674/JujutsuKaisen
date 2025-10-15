// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack/Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Attack/CustomProjectileMovement.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 0.0f;
	_LifeCountingDown = Lifespan;
	
	// CollisionSphere를 Root Component로 생성
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere); // CollisionSphere가 Root!
	CollisionSphere->SetSphereRadius(SphereRadius);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionObjectType(ECC_Projectile);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	// ProjectileMovement 컴포넌트 생성 (CollisionSphere에 부착)
	ProjectileMovement = CreateDefaultSubobject<UCustomProjectileMovement>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(CollisionSphere); // CollisionSphere를 이동 대상으로
	ProjectileMovement->InitialSpeed = 0.0f; // 초기 속도 0 (발사 전까지 정지)
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // 중력 비활성화
	ProjectileMovement->bAutoActivate = true; // BeginPlay에서 자동 활성화 (이후 Velocity로 제어)
	ProjectileMovement->Velocity = FVector::ZeroVector; // 초기 Velocity 0 (정지 상태)
	ProjectileMovement->bSweepCollision = true; // Sweep 충돌 감지
	
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// Owner는 무시하도록 설정 (Owner와 충돌 및 물리 영향 안받음)
	if (GetOwner())
	{	
		CollisionSphere->IgnoreActorWhenMoving(GetOwner(), true);
		
		// Owner를 JujutsuKaisenCharacter로 캐스팅하여 Target 가져오기
		AJujutsuKaisenCharacter* OwnerCharacter = Cast<AJujutsuKaisenCharacter>(GetOwner());
		if (OwnerCharacter)
		{
			Target = OwnerCharacter->GetTargetCharacter();
		}
	}
	
	if (ProjectileMovement && CollisionSphere)
	{
		ProjectileMovement->SetUpdatedComponent(CollisionSphere); // CollisionSphere를 이동 대상으로
	}
	
	// 오버랩 이벤트 바인딩 (Pawn과의 충돌 감지용)
	if (CollisionSphere)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
		CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnOverlapEnd);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
}