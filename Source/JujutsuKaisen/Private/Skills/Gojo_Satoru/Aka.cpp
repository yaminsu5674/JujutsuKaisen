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
}

void UAka::TickSkill(float DeltaTime)
{

}

void UAka::OnPressed()
{
	state = 1;

	if (Owner && Target)
	{
		USkillLibrary::RotateActorToFaceTarget(Owner, Target);
	}
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

		if (AkaEarlyMontage)
		{
			AnimInstance->Montage_Play(AkaEarlyMontage);
			if (!AkaProjectile)
			{
				SpawnProjectile();
			}
		}
		return;
	}
	else if (AnimInstance && state == 2)
	{
		//LaunchProjectile();
		AnimInstance->Montage_Resume(AkaLateMontage);
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
		// 기존 바인딩 제거 (중복 방지)
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify2Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify3Begin);

		// 새로 바인딩
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify2Begin);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify3Begin);
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
			SpawnProjectile();
		}
	}
}

void UAka::OnMontageNotify3Begin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("AkaNotify3"))
	{
		// 아카 프로젝타일의 현재 업데이티드컴포넌트와 벨로시티 출력해줘
		LaunchProjectile();
	}
}

void UAka::SpawnProjectile()
{
	if (!ProjectileClass || !Owner) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;

	const FVector ForwardOffset = FVector(60.f, 0.f, 0.f);
	const FRotator SpawnRotation = Owner->GetActorRotation();
	const FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(FName("index_03_r")) + Owner->GetActorForwardVector() * 10.0f;

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

	// 발사체가 유효한지 확인
	if (AkaProjectile)
	{
		// 먼저 방향 설정
		AkaProjectile->SetDirection();
		// 그 다음 Move 상태로 전환 (Velocity 활성화)
		AkaProjectile->SetBehaviorType(EProjectileBehaviorType::Move);
		// 중복 호출 제거
		AkaProjectile = nullptr;
	}
}

