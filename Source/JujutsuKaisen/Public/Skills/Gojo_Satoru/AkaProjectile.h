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

	// 부모의 OnOverlapBegin 함수 오버라이드
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 부모의 Tick 함수 오버라이드
	virtual void Tick(float DeltaTime) override;

	// BeginPlay 오버라이드
	virtual void BeginPlay() override;

protected:
	// 파티클 시스템들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ChargingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ShotEffect;

	// ChargingEffect 컴포넌트 참조 (제거용)
	UPROPERTY()
	UParticleSystemComponent* ChargingEffectComponent;
};
