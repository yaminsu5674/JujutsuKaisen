// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attack/Projectile.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "MurasakiProjectile.generated.h"

UCLASS()
class JUJUTSUKAISEN_API AMurasakiProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AMurasakiProjectile();

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
};
