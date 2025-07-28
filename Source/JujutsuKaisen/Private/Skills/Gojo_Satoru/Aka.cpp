// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/Aka.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

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

void UAka::OnPressed(AJujutsuKaisenCharacter* Target)
{
	state = 1;

	if (AnimInstance && AkaLateMontage)
	{
		BindMontageNotifies();
		AnimInstance->Montage_Play(AkaLateMontage);
	}

}

void UAka::OnReleased(AJujutsuKaisenCharacter* Target)
{
	if (AnimInstance && state == 1)
	{
		AnimInstance->Montage_Stop(0.2f);

		if ( AkaEarlyMontage)
		{
			AnimInstance->Montage_Play(AkaEarlyMontage);
			if (!AkaProjectile)
			{
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

// 초기화 함수
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

// 노티파이 바인딩
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

// 노티파이 언바인딩
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

	const FVector Offset = Owner->GetActorForwardVector() * 10.f;
	const FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(FName("hand_r")) + Offset;
	const FRotator SpawnRotation = Owner->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner;

	AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (!Projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Aka projectile"));
		return;
	}

	Projectile->InitializeParams();

	Projectile->SetActorEnableCollision(false);
	/*Projectile->AttachToComponent(
		Owner->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName("hand_r")
	);*/

	AkaProjectile = Projectile;
}


void UAka::LaunchProjectile()
{
	if (AkaProjectile)
	{
		AkaProjectile->SetActorEnableCollision(true);
		AkaProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AkaProjectile->SetBehaviorType(EProjectileBehaviorType::Move);
		AkaProjectile = nullptr;
	}
}

