// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CustomProjectileMovement.generated.h"

/**
 * Custom Projectile Movement Component
 * ProjectileMovementComponent를 상속받아 커스텀 동작 구현
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUJUTSUKAISEN_API UCustomProjectileMovement : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	UCustomProjectileMovement();

protected:
	// 부모 클래스의 가상 함수 오버라이드
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
	// 충돌 시 멈추지 않고 계속 진행하도록 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Projectile")
	bool bNeverStop;
};

