// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Sukuna/Pal.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Attack/Projectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Skills/Sukuna/PalProjectile.h"
#include "Library/SkillLibrary.h"

UPal::UPal()
{
	state = 0;
}

void UPal::TickSkill(float DeltaTime)
{
	Super::TickSkill(DeltaTime);
}

void UPal::OnPressed()
{
	Super::OnPressed();
    if (Owner && Target)
    {
        USkillLibrary::RotateActorToFaceTarget(Owner, Target);
    }

    if (AnimInstance && PalMontage)
	{
		BindMontageNotifies();
		AnimInstance->Montage_Play(PalMontage);
	}

}


void UPal::ResetSkill()
{
	Super::ResetSkill();
	
	UnbindMontageNotifies();
	
	if (PalProjectile)
	{
		PalProjectile->Destroy();
		PalProjectile = nullptr;
	}
}

void UPal::BindMontageNotifies()
{
	if (AnimInstance)
	{
		// Pal 몽타주 노티파이 바인딩
		if (PalMontage)
		{
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UPal::OnMontageNotify1Begin);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pal: PalMontage가 null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pal: AnimInstance가 null"));
	}
}

void UPal::UnbindMontageNotifies()
{
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UPal::OnMontageNotify1Begin);
	}
}

void UPal::SpawnProjectile()
{
	if (!ProjectileClass || !Owner) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;

	// 캐릭터 기준 전방 300, 높이 100 위치에 스폰
	const FVector ForwardOffset = FVector(200.f, 0.f, 88.f);
	const FRotator SpawnRotation = Owner->GetActorRotation();
	const FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * ForwardOffset.X + FVector(0.f, 0.f, ForwardOffset.Z);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner;

	PalProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (!PalProjectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Pal projectile"));
		return;
	}

	PalProjectile->SetBehaviorType(EProjectileBehaviorType::Place);
    PalProjectile->CheckOverlap();
}



void UPal::OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

	if (NotifyName == FName("PalNotify1"))
	{

		// 몽타주 일시정지
		if (AnimInstance && PalMontage)
		{
			AnimInstance->Montage_Pause(PalMontage);

		}
		
		// 프로젝타일이 없거나 파괴된 상태인지 확인
		if (!PalProjectile || !IsValid(PalProjectile))
		{
			SpawnProjectile();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pal: 프로젝타일이 이미 존재함 - 스폰 건너뜀"));
		}

		// 2초 후 EndPal 호출
		if (Owner)
		{
			
			FTimerHandle TimerHandle;
			Owner->GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (PalProjectile && IsValid(PalProjectile))
				{
					// PalProjectile의 EndPal 함수 호출
					APalProjectile* PalProj = Cast<APalProjectile>(PalProjectile);
					if (PalProj)
					{
						
						PalProj->EndPal();
					}
				}
                if (AnimInstance && PalMontage)
                {
                    AnimInstance->Montage_Resume(PalMontage);
                }
                PalProjectile = nullptr;    
			}, 2.0f, false);
		}
	}
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Pal: 알 수 없는 노티파이 - %s"), *NotifyName.ToString());
    }
}

