// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attack/CustomProjectileMovement.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Projectile.generated.h"

UENUM(BlueprintType)
enum class EProjectileBehaviorType : uint8
{
	None,
	Move,
	Place
};

UCLASS()
class JUJUTSUKAISEN_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Projectile Params")
	EProjectileBehaviorType BehaviorType = EProjectileBehaviorType::None;

	//bool bIsCharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float Speed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float Lifespan = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float SphereRadius = 50.0f;

	UPROPERTY()
	float _LifeCountingDown;

	UPROPERTY()
	FVector Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UCustomProjectileMovement* ProjectileMovement;

	// 오버랩 상태 확인 변수
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsOverlapping = false;

	// 오버랩된 타겟 캐릭터
	UPROPERTY(BlueprintReadOnly, Category = "State")
	AJujutsuKaisenCharacter* Target = nullptr;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void HandleMovement(float DeltaTime);

	void SetDirection(AJujutsuKaisenCharacter* InTarget = nullptr);

	void SetTarget(AJujutsuKaisenCharacter* InTarget);

	void SetBehaviorType(EProjectileBehaviorType NewType);

	void CheckOverlap();

	// 오버랩 이벤트 함수들
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return _MeshComponent;
	}

};
