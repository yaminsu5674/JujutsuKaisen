// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/AkaProjectile.h"

AAkaProjectile::AAkaProjectile()
{

}

void AAkaProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Target = Cast<AJujutsuKaisenCharacter>(OtherActor);
	if (Target != nullptr)
	{
		// 캐릭터에게 데미지 적용
		Target->Hit();
		
		// 물리 충돌로 캐릭터 날리기
		if (Target->GetCharacterMovement())
		{
			FVector KnockbackDirection = Direction; // 발사체 방향으로 날리기
			FVector KnockbackForce = KnockbackDirection * 2000.0f;
			
			// LaunchCharacter로 캐릭터 발사 (수평으로만)
			Target->LaunchCharacter(KnockbackForce, false, true);
		}
		
		PrimaryActorTick.bCanEverTick = false;
		Destroy();
	}
}
