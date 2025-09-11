// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/AkaProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"

AAkaProjectile::AAkaProjectile()
{

}

void AAkaProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부모의 OnOverlapBegin 호출 (Target 초기화)
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (Target != nullptr && !bIsOverlapping)
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
            
            // 방법 1: LaunchCharacter (속도 기반)
            // FVector KnockbackForce = LaunchDir * 1500.f + FVector(0, 0, 200.f);
            // Target->LaunchCharacter(KnockbackForce, true, true);
            
            // 방법 2: AddImpulse (힘 기반) - 주석 해제하여 사용 가능
            FVector ImpulseForce = LaunchDir * 4000.f + FVector(0, 0, 1000.f);
            Target->GetCharacterMovement()->AddImpulse(ImpulseForce, true);
            
            // 방법 3: Velocity 직접 설정 (가장 부드러움) - 주석 해제하여 사용 가능
            // FVector NewVelocity = LaunchDir * 800.f + FVector(0, 0, 150.f);
            // Target->GetCharacterMovement()->Velocity = NewVelocity;
        }
		bIsOverlapping = true;
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
