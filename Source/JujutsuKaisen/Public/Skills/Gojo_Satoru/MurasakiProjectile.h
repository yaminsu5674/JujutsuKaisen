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

	// 부모의 OnOverlapBegin 함수 오버라이드
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// BeginPlay 오버라이드
	virtual void BeginPlay() override;

	// Tick 오버라이드
	virtual void Tick(float DeltaTime) override;

protected:
	// 나이아가라 시스템 (차징 이펙트용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ChargingNiagaraEffect;

	// 파티클 시스템 (샷 이펙트용 - 기존 유지)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ShotEffect;

	// 나이아가라 컴포넌트 (차징 이펙트용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* ChargingNiagaraComponent;

	// 크기 스케일 관련 (디버깅용)
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	float CurrentScale = 1.0f;
};
