// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/BaseSkill.h"
#include "Attack/Projectile.h"
#include "Animation/AnimMontage.h"
#include "Aka.generated.h"

UCLASS()
class JUJUTSUKAISEN_API UAka : public UBaseSkill
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 state;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AkaEarlyMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AkaLateMontage;

	UPROPERTY()
	AProjectile* AkaProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

public:
	UAka();

	virtual void TickSkill(float DeltaTime) override;

	virtual void OnPressed() override;

	virtual void OnReleased() override;

	// 초기화 및 바인딩 관리
	virtual void ResetSkill() override;
	void BindMontageNotifies();
	void UnbindMontageNotifies();

	// 발사체 생성 및 발사 처리
	void SpawnProjectile();
	void LaunchProjectile();

	// 노티파이 핸들러
	UFUNCTION()
	void OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OnMontageNotify2Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OnMontageNotify3Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};


