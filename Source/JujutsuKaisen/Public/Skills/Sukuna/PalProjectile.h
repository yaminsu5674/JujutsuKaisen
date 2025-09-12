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

	// 부모의 OnOverlapBegin 함수 오버라이드
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 부모의 OnOverlapEnd 함수 오버라이드
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// BeginPlay 오버라이드
	virtual void BeginPlay() override;

	// Tick 오버라이드
	virtual void Tick(float DeltaTime) override;

	void EndPal();

protected:
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
