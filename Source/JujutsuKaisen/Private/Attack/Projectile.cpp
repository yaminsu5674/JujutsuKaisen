// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack/Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Attack/CustomProjectileMovement.h"
#include "Components/SphereComponent.h"
#include "Library/SkillLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 수명 설정: Lifespan 초 후 자동 소멸
	//InitialLifeSpan = Lifespan;  // 5초 후 자동 소멸 (언리얼 내장 시스템)
	Lifespan = 5.0f;
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
	CheckOverlap();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when actor is explicitly destroyed
void AProjectile::Destroyed()
{
	// 발사체 소멸 시 공통 정리 로직
	UE_LOG(LogTemp, Warning, TEXT("Projectile Destroyed: %s"), *GetName());
	
	// 자식 클래스에서 소멸 이펙트 추가 가능 (오버라이드)
	
	Super::Destroyed();
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Owner는 무시 (자신의 스킬에 맞지 않도록)
	if (OtherActor == GetOwner())
	{
		return;
	}
	
	// 오버랩된 액터를 캐스팅하여 Target과 비교
	AJujutsuKaisenCharacter* OverlappedCharacter = Cast<AJujutsuKaisenCharacter>(OtherActor);
	
	// Target과 일치하면 bIsOverlapping = true
	if (OverlappedCharacter && OverlappedCharacter == Target)
	{
		bIsOverlapping = true;
		
		// 자식 클래스의 커스텀 로직 호출
		OnProjectileOverlapBegin(OtherActor);
	}
}

void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Target과의 오버랩이 끝나면 bIsOverlapping = false
	AJujutsuKaisenCharacter* OverlappedCharacter = Cast<AJujutsuKaisenCharacter>(OtherActor);
	
	if (OverlappedCharacter && OverlappedCharacter == Target)
	{
		bIsOverlapping = false;
		
		// 자식 클래스의 커스텀 로직 호출
		OnProjectileOverlapEnd(OtherActor);
	}
}

void AProjectile::OnProjectileOverlapBegin(AActor* OtherActor)
{
	// 기본 구현 없음 (자식 클래스에서 오버라이드)
}

void AProjectile::OnProjectileOverlapEnd(AActor* OtherActor)
{
	// 기본 구현 없음 (자식 클래스에서 오버라이드)
}

void AProjectile::CheckOverlap()
{
	if (Target)
	{
		if (CollisionSphere->IsOverlappingActor(Target))
		{
			bIsOverlapping = true;
			if (Target->GetStateManager() && GetOwner())
			{
				bool bIsHitFront = USkillLibrary::JudgeHitFront(GetOwner(), Target);
				Target->GetStateManager()->SetHitSubState(EHitSubState::Stun, bIsHitFront);
			}	
			UE_LOG(LogTemp, Error, TEXT("Already Overlapping: %s"), *Target->GetName());

			// 강제로 OnOverlapBegin 흐름을 태워 자식 클래스 커스텀 로직 실행
			FHitResult DummyHit;
			OnOverlapBegin(CollisionSphere, Target, nullptr, 0, false, DummyHit);
		}
	}
}