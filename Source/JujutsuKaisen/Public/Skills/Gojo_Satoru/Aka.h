// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/BaseSkill.h"
#include "Attack/Projectile.h"
#include "Animation/AnimMontage.h"
#include "Aka.generated.h"

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class JUJUTSUKAISEN_API UAka : public UBaseSkill
{
	GENERATED_BODY()

	uint8 state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AkaEarlyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AkaLateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY()
	AProjectile* AkaProjectile;

	

public:
	UAka();

	virtual void TickSkill(float DeltaTime) override;

	virtual void OnPressed() override;

	virtual void OnReleased() override;

	// �ʱ�ȭ �� ���ε� ����
	virtual void ResetSkill() override;
	void BindMontageNotifies();
	void UnbindMontageNotifies();

	// �߻�ü ���� �� �߻� ó��
	void SpawnProjectile();
	void LaunchProjectile();

	// ��Ƽ���� �ڵ鷯
	UFUNCTION()
	void OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OnMontageNotify2Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OnMontageNotify3Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};


