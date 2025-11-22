// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attack/Projectile.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
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

	// Destroyed 오버라이드 (소멸 시 특수 이펙트)
	virtual void Destroyed() override;

	// ChargingEffect를 붙이는 함수
	void AttachChargingEffect();

	// HitSphere용 오버랩 함수 (가상함수 아님)
	UFUNCTION()
	void OnHitSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnHitSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* HitSphere;

	// HitSphere 반지름 (블루프린트에서 수정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	float HitSphereRadius;
	
	// 파티클 시스템들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ChargingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ShotEffect;

	// ChargingEffect 컴포넌트 참조 (제거용)
	UPROPERTY()
	UParticleSystemComponent* ChargingEffectComponent;
	
	// 나이아가라 이펙트 시스템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* MurasakiNiagaraEffect;
	
	// 나이아가라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* MurasakiNiagaraComponent;
};
