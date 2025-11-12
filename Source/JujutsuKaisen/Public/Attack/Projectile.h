// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attack/CustomProjectileMovement.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Projectile.generated.h"

// 커스텀 콜리전 채널 (프로젝트 설정에서 생성한 Projectile 채널)
#define ECC_Projectile ECC_GameTraceChannel1

UCLASS()
class JUJUTSUKAISEN_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float Speed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float Lifespan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float SphereRadius = 50.0f;

	UPROPERTY()
	float _LifeCountingDown;

	// CollisionSphere를 Root Component로 사용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	// ProjectileMovement (CollisionSphere에 부착됨)
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

	// Called when actor is explicitly destroyed (소멸 시 이펙트 연출용)
	virtual void Destroyed() override;

public:	
	// Sets default values for this actor's properties
	AProjectile();

	void CheckOverlap();
	
	// ProjectileMovement getter
	FORCEINLINE UCustomProjectileMovement* GetProjectileMovement() const { return ProjectileMovement; }
	
	// Target getter
	FORCEINLINE AJujutsuKaisenCharacter* GetTarget() const { return Target; }
	
	// Speed getter
	FORCEINLINE float GetSpeed() const { return Speed; }
	
	// Lifespan getter
	FORCEINLINE float GetLifespan() const { return Lifespan; }
	
	// CollisionSphere getter
	FORCEINLINE USphereComponent* GetCollisionSphere() const { return CollisionSphere; }

	// 오버랩 이벤트 함수들 (final - 자식에서 오버라이드 불가)
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// 자식 클래스에서 오버라이드할 가상 함수들
	virtual void OnProjectileOverlapBegin(AActor* OtherActor);
	
	virtual void OnProjectileOverlapEnd(AActor* OtherActor);

};
