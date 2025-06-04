// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Gojo_Satoru/Aka.h"

UAka::UAka()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(TEXT("AnimMontage'/Game/Static/Animation_Source/Aka_Montage.Aka_Montage'"));
	if (MontageFinder.Succeeded())
	{
		AkaMontage = MontageFinder.Object;
	}

	static ConstructorHelpers::FClassFinder<AProjectile> ProjectileBPClass(TEXT("/Game/Dynamic/Projectiles/BP_Aka")); // ��δ� �� ������Ʈ ������ ���� ����
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

			FVector ForwardOffset = Caster->GetActorForwardVector() * 50.f;  // 50cm ��
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
				// ���÷� �ӵ�, ���ӽð�, ������, Ÿ���� �ʱ�ȭ(�ʰ� ���ϴ� ������ �ٲ㼭 ���)
				SpawnedProjectile->Initialize();
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,                   // Key (-1�̸� ��� ���ο� �޽����� ǥ��)
						5.f,                  // Duration (ȭ�鿡 �� �� ���� ǥ������)
						FColor::Red,          // ����
						TEXT("Aka") // ����� ���ڿ�
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