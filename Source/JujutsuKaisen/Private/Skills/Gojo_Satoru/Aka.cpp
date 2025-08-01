// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/Aka.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Library/SkillLibrary.h"

UAka::UAka()
{
	bWantsTick = true;
	AkaProjectile = nullptr;
	state = 0;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EarlyMontageFinder(TEXT("AnimMontage'/Game/Static/JujutsuKaisenCharacters/GojoSatoru_Student/Animation/AkaEarly_Montage.AkaEarly_Montage'"));
	if (EarlyMontageFinder.Succeeded()) AkaEarlyMontage = EarlyMontageFinder.Object;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LateMontageFinder(TEXT("AnimMontage'/Game/Static/JujutsuKaisenCharacters/GojoSatoru_Student/Animation/AkaLate_Montage.AkaLate_Montage'"));
	if (LateMontageFinder.Succeeded()) AkaLateMontage = LateMontageFinder.Object;

	static ConstructorHelpers::FClassFinder<AProjectile> ProjectileBPClass(TEXT("/Game/Dynamic/Blueprints/Projectiles/BP_Projectile_Aka"));
	if (ProjectileBPClass.Succeeded()) ProjectileClass = ProjectileBPClass.Class;
}

void UAka::TickSkill(float DeltaTime)
{
	// 충전 등 지속 로직
}

void UAka::OnPressed()
{
	state = 1;

	if (AnimInstance && AkaLateMontage)
	{
		BindMontageNotifies();
		AnimInstance->Montage_Play(AkaLateMontage);
	}

}

void UAka::OnReleased()
{
	if (AnimInstance && state == 1)
	{
		AnimInstance->Montage_Stop(0.2f);

		if ( AkaEarlyMontage)
		{
			AnimInstance->Montage_Play(AkaEarlyMontage);
			if (!AkaProjectile)
			{
				UE_LOG(LogTemp, Error, TEXT("SpawnProjectile()"));
				SpawnProjectile();
			}
		}
		return;
	}
	if (AnimInstance && state == 2)
	{
		UnbindMontageNotifies();
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify3Begin);
		AnimInstance->Montage_Resume(AkaLateMontage);
		LaunchProjectile();
	}
}

void UAka::ResetSkill()
{
	state = 0;

	if (AkaProjectile)
	{
		AkaProjectile->Destroy();
		AkaProjectile = nullptr;
	}
	UnbindMontageNotifies();
}

void UAka::BindMontageNotifies()
{
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify2Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify3Begin);

		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify2Begin);
	}
}

void UAka::UnbindMontageNotifies()
{
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify2Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify3Begin);
	}
}

void UAka::OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("AkaNotify1"))
	{
		LaunchProjectile();
	}
}

void UAka::OnMontageNotify2Begin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("AkaNotify2"))
	{
		state = 2;
		AnimInstance->Montage_Pause(AkaLateMontage);
		if (!AkaProjectile)
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnProjectile()"));
			SpawnProjectile();
		}
	}
}

void UAka::OnMontageNotify3Begin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("AkaNotify3"))
	{

	}
}

void UAka::SpawnProjectile()
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
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Aka projectile"));
		return;
	}

	AkaProjectile = Projectile;
}


void UAka::LaunchProjectile()
{
	if (Owner && Target)
	{
		USkillLibrary::RotateActorToFaceTarget(Owner, Target);
	}
	if (AkaProjectile)
	{
		AkaProjectile->SetDirection(Target);
		AkaProjectile->SetBehaviorType(EProjectileBehaviorType::Move);
		AkaProjectile = nullptr;
	}
}

