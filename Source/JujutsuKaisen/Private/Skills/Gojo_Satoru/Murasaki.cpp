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
	bIsGrowing = false;
	GrowthTimer = 0.0f;
	GrowthDuration = 1.0f;
	GrowthRate = 1.0f;
}

void UMurasaki::TickSkill(float DeltaTime)
{
	// 크기 증가 중일 때 프로젝타일 크기 증가
	if (bIsGrowing && MurasakiProjectile)
	{
		GrowthTimer += DeltaTime;
		
		// 크기 증가 (1초 동안 지속적으로 증가)
		float GrowthProgress = GrowthTimer / GrowthDuration;
		float CurrentScale = 1.0f + (GrowthRate * GrowthProgress);
		
		MurasakiProjectile->SetActorScale3D(FVector(CurrentScale, CurrentScale, CurrentScale));
		
		// 1초가 지나면 크기 증가 중단
		if (GrowthTimer >= GrowthDuration)
		{
			StopGrowth();
		}
	}
}

void UMurasaki::OnPressed()
{
	state = 1;
	GrowthDuration = 1.5f;

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
	bIsGrowing = false;
	GrowthTimer = 0.0f;

	// 타이머 클리어
	if (Owner && Owner->GetWorld())
	{
		Owner->GetWorld()->GetTimerManager().ClearTimer(GrowthTimerHandle);
	}

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
		// 몽타주 일시정지
		if (AnimInstance && MurasakiMontage)
		{
			AnimInstance->Montage_Pause(MurasakiMontage);
		}
		
		// 프로젝타일이 없거나 파괴된 상태인지 확인
		if (!MurasakiProjectile || !IsValid(MurasakiProjectile))
		{
			SpawnProjectile();
			
			// 프로젝타일이 스폰되면 크기 증가 시작
			if (MurasakiProjectile)
			{
				StartGrowth();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Murasaki: 프로젝타일이 이미 존재함 - 스폰 건너뜀"));
		}
	}
}

void UMurasaki::SpawnProjectile()
{
	if (!ProjectileClass || !Owner) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;

	// 캐릭터 기준 전방 1000, 높이 500 위치에 스폰
	const FVector ForwardOffset = FVector(300.f, 0.f, 80.f);
	const FRotator SpawnRotation = Owner->GetActorRotation();
	const FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * ForwardOffset.X + FVector(0.f, 0.f, ForwardOffset.Z);

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
		
		// 발사 후 참조 정리 (다음 스킬 사용을 위해)
		MurasakiProjectile = nullptr;
	}
	
	// 몽타주 재개
	if (AnimInstance && MurasakiMontage)
	{
		AnimInstance->Montage_Resume(MurasakiMontage);
	}
}

// ============================================================================
// 크기 증가 관련 함수들
// ============================================================================

void UMurasaki::StartGrowth()
{
	bIsGrowing = true;
	GrowthTimer = 0.0f;
}

void UMurasaki::StopGrowth()
{
	bIsGrowing = false;
	
	// 크기 증가가 완료되면 발사
	if (MurasakiProjectile)
	{
		LaunchProjectile();
	}
}

