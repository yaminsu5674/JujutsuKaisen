// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/AkaProjectile.h"

AAkaProjectile::AAkaProjectile()
{

}

void AAkaProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부모의 OnOverlapBegin 호출 (bIsOverlapping = true, Target 초기화)
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (Target != nullptr)
	{
		// 캐릭터에게 데미지 적용
		if (Target->GetStateManager())
		{
			Target->GetStateManager()->SetHitSubState(EHitSubState::MediumHit);
		}
		
		// 물리 충돌로 캐릭터 날리기
		if (Target->GetCharacterMovement())
        {
            // Projectile의 방향 벡터 (정규화된)
            FVector LaunchDir = GetActorForwardVector();

            // 세기 조절 (앞으로 600, 위로 300 정도)
            FVector KnockbackForce = LaunchDir * 6000.f + FVector(0, 0, 500.f);

            // LaunchCharacter 호출
            Target->LaunchCharacter(KnockbackForce, true, true);
        }
	}
}

void AAkaProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Aka 전용 Tick 로직
	if (bIsOverlapping && Target != nullptr)
	{
		// 오버랩 중일 때의 로직
		// 필요시 여기에 지속적인 데미지나 효과 추가 가능
		Target->Hit();
	}
	
}
