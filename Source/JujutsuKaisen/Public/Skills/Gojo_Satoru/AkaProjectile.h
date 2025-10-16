// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attack/Projectile.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "AkaProjectile.generated.h"

UCLASS()
class JUJUTSUKAISEN_API AAkaProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AAkaProjectile();

protected:
	// 부모의 가상 함수 오버라이드
	virtual void OnProjectileOverlapBegin(AActor* OtherActor) override;
	
	virtual void OnProjectileOverlapEnd(AActor* OtherActor) override;

public:

	// BeginPlay 오버라이드
	virtual void BeginPlay() override;

	// Tick 오버라이드
	virtual void Tick(float DeltaTime) override;

protected:
	// 메시 컴포넌트 (시각적 표현용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
	// 파티클 시스템들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ChargingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ShotEffect;

	// ChargingEffect 컴포넌트 참조 (제거용)
	UPROPERTY()
	UParticleSystemComponent* ChargingEffectComponent;

	// ShotEffect 생성 간격 타이머
	UPROPERTY()
	float ShotEffectTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float ShotEffectInterval;
};
