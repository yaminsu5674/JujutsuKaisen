// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/Murasaki.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Library/SkillLibrary.h"

UMurasaki::UMurasaki()
{
	bWantsTick = true;
	MurasakiProjectile = nullptr;
	state = 0;
}

void UMurasaki::TickSkill(float DeltaTime)
{
	// 스킬 틱 로직
}

void UMurasaki::OnPressed()
{
	state = 1;

	if (Owner && Target)
	{
		USkillLibrary::RotateActorToFaceTarget(Owner, Target);
	}
	if (AnimInstance && MurasakiMontage)
	{
		BindMontageNotifies();
		AnimInstance->Montage_Play(MurasakiMontage);
	}
}

void UMurasaki::ResetSkill()
{
	state = 0;

	if (MurasakiProjectile)
	{
		MurasakiProjectile->Destroy();
		MurasakiProjectile = nullptr;
	}
	UnbindMontageNotifies();
}

void UMurasaki::BindMontageNotifies()
{
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMurasaki::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMurasaki::OnMontageNotify1Begin);
	}
}

void UMurasaki::UnbindMontageNotifies()
{
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMurasaki::OnMontageNotify1Begin);
	}
}

void UMurasaki::OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("MurasakiNotify1"))
	{
		if (!MurasakiProjectile)
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnProjectile()"));
			SpawnProjectile();
		}
		LaunchProjectile();
	}
}

void UMurasaki::SpawnProjectile()
{
	if (!ProjectileClass || !Owner) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;

	const FVector ForwardOffset = FVector(60.f, 0.f, 0.f);
	const FRotator SpawnRotation = Owner->GetActorRotation();
	const FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(FName("index_03_r")) + Owner->GetActorForwardVector();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner;

	AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (!Projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Murasaki projectile"));
		return;
	}

	MurasakiProjectile = Projectile;
}


void UMurasaki::LaunchProjectile()
{
	if (Owner && Target)
	{
		USkillLibrary::RotateActorToFaceTarget(Owner, Target);
	}
	if (MurasakiProjectile)
	{
		MurasakiProjectile->SetDirection(Target);
		MurasakiProjectile->SetBehaviorType(EProjectileBehaviorType::Move);
		MurasakiProjectile = nullptr;
	}
}
