// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attack/Projectile.h"
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
};
