// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Gojo_Satoru/Aka.h"

UAka::UAka()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(TEXT("AnimMontage'/Game/Static/Animation_Source/Aka_Montage.Aka_Montage'"));
	if (MontageFinder.Succeeded())
	{
		AkaMontage = MontageFinder.Object;
	}

	static ConstructorHelpers::FClassFinder<AProjectile> ProjectileBPClass(TEXT("/Game/Dynamic/Projectiles/BP_Aka")); // 경로는 니 프로젝트 구조에 따라 수정
	if (ProjectileBPClass.Succeeded())
	{
		ProjectileClass = ProjectileBPClass.Class;
	}
}

void UAka::Activate(AJujutsuKaisenCharacter* Caster, AJujutsuKaisenCharacter* Target)
{
	Super::Activate(Caster, Target);

	// activate collision on Castser's HitBox;
	if (Caster)
	{
		Caster->ActivateAttack();
	}


	// Spawn
	if (ProjectileClass)
	{
		UWorld* World = Caster->GetWorld();
		if (World)
		{

			FVector ForwardOffset = Caster->GetActorForwardVector() * 50.f;  // 50cm 앞
			FVector SpawnLocation = Caster->GetMesh()->GetSocketLocation(FName("hand_L")) + ForwardOffset;
			FRotator SpawnRotation = Caster->GetActorRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Caster;
			SpawnParams.Instigator = Caster;

			AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);


			if (!SpawnedProjectile)
			{
				UE_LOG(LogTemp, Error, TEXT("SpawnActor failed"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Projectile spawned at: %s"), *SpawnedProjectile->GetActorLocation().ToString());
			}

			if (SpawnedProjectile)
			{
				// 예시로 속도, 지속시간, 데미지, 타겟을 초기화(너가 원하는 값으로 바꿔서 사용)
				SpawnedProjectile->Initialize();
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,                   // Key (-1이면 계속 새로운 메시지로 표시)
						5.f,                  // Duration (화면에 몇 초 동안 표시할지)
						FColor::Red,          // 색상
						TEXT("Aka") // 출력할 문자열
					);
				}
			}
		}
	}

	// set HitBox's damage with damage member
	UAnimInstance* AnimInstance = Caster->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// play animMontage with INTERFFACE!!!!!!!!!!!!!!!!!!!!
	if (AnimInstance && AkaMontage)
	{
		AnimInstance->Montage_Play(AkaMontage);
	}
	


}