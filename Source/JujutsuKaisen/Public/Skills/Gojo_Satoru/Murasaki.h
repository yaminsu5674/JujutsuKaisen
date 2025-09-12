// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/BaseSkill.h"
#include "Attack/Projectile.h"
#include "Animation/AnimMontage.h"
#include "Murasaki.generated.h"

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class JUJUTSUKAISEN_API UMurasaki : public UBaseSkill
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MurasakiMontage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY()
	AProjectile* MurasakiProjectile;

	

public:
	UMurasaki();

	virtual void TickSkill(float DeltaTime) override;

	virtual void OnPressed() override;

	// 초기화 및 리셋 함수
	virtual void ResetSkill() override;
	void BindMontageNotifies();
	void UnbindMontageNotifies();

	// 발사체 생성 및 발사 처리
	void SpawnProjectile();
	void LaunchProjectile();

	// 몽타주 노티파이
	UFUNCTION()
	void OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};
