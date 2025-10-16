// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attack/Projectile.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "PalProjectile.generated.h"

UCLASS()
class JUJUTSUKAISEN_API APalProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	APalProjectile();

protected:
	// 부모의 가상 함수 오버라이드
	virtual void OnProjectileOverlapBegin(AActor* OtherActor) override;
	
	virtual void OnProjectileOverlapEnd(AActor* OtherActor) override;

public:

	// BeginPlay 오버라이드
	virtual void BeginPlay() override;

	// Tick 오버라이드
	virtual void Tick(float DeltaTime) override;

	void EndPal();

protected:
	// 메시 컴포넌트 (시각적 표현용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
	// 나이아가라 시스템 (차징 이펙트용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ChargingNiagaraEffect;

	// 파티클 시스템 (샷 이펙트용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ShotEffect;

	// 나이아가라 컴포넌트 (차징 이펙트용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* ChargingNiagaraComponent;
};
