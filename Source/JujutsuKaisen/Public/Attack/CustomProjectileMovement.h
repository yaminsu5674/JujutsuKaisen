// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CustomProjectileMovement.generated.h"

UENUM(BlueprintType)
enum class EProjectileMoveType : uint8
{
	Single,		// 단발형: 한 번 발사되어 날아가는 타입
	Rush,		// 돌진형: 계속 추진력을 가지고 돌진하는 타입
};

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
	// 발사체 이동 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Projectile")
	EProjectileMoveType MoveType;

protected:
	// 부모 클래스의 가상 함수 오버라이드
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	// 충돌 처리 함수 오버라이드
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice = 0.f, const FVector& MoveDelta = FVector::ZeroVector) override;
	
	// 블로킹 충돌 처리 오버라이드
	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;

public:
	// 이동 타입 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Custom Projectile")
	void SetMoveType(EProjectileMoveType NewMoveType);
	
	// 충돌 시 멈추지 않고 계속 진행하도록 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Projectile")
	bool bNeverStop;
};

