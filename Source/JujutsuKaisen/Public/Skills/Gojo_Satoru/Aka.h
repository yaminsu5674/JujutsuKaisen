// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/BaseSkill.h"
#include "Attack/Projectile.h"
#include "Aka.generated.h"

/**
 * 
 */
UCLASS()
class JUJUTSUKAISEN_API UAka : public UBaseSkill
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AkaMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

public:
	UAka();

	virtual void Activate(AJujutsuKaisenCharacter* Caster, AJujutsuKaisenCharacter* Target = nullptr) override;
	
};
