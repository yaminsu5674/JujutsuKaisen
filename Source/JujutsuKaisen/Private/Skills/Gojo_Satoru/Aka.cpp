// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Gojo_Satoru/Aka.h"

UAka::UAka()
{
	bWantsTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(TEXT("AnimMontage'/Game/Static/Animation_Source/Aka_Montage.Aka_Montage'"));
	if (MontageFinder.Succeeded())
	{
		AkaMontage = MontageFinder.Object;
	}

	static ConstructorHelpers::FClassFinder<AProjectile> ProjectileBPClass(TEXT("/Game/Dynamic/Projectiles/BP_Projectile_Aka")); 
	if (ProjectileBPClass.Succeeded())
	{
		ProjectileClass = ProjectileBPClass.Class;
	}
}


void UAka::TickSkill(float DeltaTime)
{

}


void UAka::OnPressed(AJujutsuKaisenCharacter* Target)
{
	// Spawn
	if (ProjectileClass)
	{
		UWorld* World = Owner->GetWorld();
		if (World)
		{

			FVector ForwardOffset = Owner->GetActorForwardVector() * 50.f;  // 50cm 앞
			FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(FName("hand_L")) + ForwardOffset;
			FRotator SpawnRotation = Owner->GetActorRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Owner;
			SpawnParams.Instigator = Owner;

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
				SpawnedProjectile->InitializeParams();
				SpawnedProjectile->SetBehaviorType(EProjectileBehaviorType::Move);
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
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// play animMontage with INTERFFACE!!!!!!!!!!!!!!!!!!!!
	if (AnimInstance && AkaMontage)
	{
		AnimInstance->Montage_Play(AkaMontage);
	}
}

void UAka::OnReleased(AJujutsuKaisenCharacter* Target)
{

}


