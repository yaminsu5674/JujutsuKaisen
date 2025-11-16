// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/BaseSkill.h"
#include "Attack/Projectile.h"
#include "Animation/AnimMontage.h"
#include "Pal.generated.h"

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class JUJUTSUKAISEN_API UPal : public UBaseSkill
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PalMontage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY()
	AProjectile* PalProjectile;

public:
	UPal();

	virtual void TickSkill(float DeltaTime) override;

	virtual void OnPressed() override;

	// 초기화 및 상태 리셋 함수
	virtual void ResetSkill() override;
	void BindMontageNotifies();
	void UnbindMontageNotifies();

	// 발사체 생성 및 발사 처리
	void SpawnProjectile();


	// 몽타주 노티파이 바인딩
	UFUNCTION()
	void OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

};
