// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Projectile.generated.h"

UENUM(BlueprintType)
enum class EProjectileBehaviorType : uint8
{
	None,
	Move,
	Place
};

UCLASS()
class JUJUTSUKAISEN_API AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Projectile Params")
	EProjectileBehaviorType BehaviorType = EProjectileBehaviorType::None;

	//bool bIsCharging = false;

	UPROPERTY(EditAnywhere, Category = "Projectile Params")
	float Speed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile Params")
	float Lifespan = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile Params")
	float Damage = 10.0f;

	float _LifeCountingDown;

	FVector Direction;

	AJujutsuKaisenCharacter *TargetCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _MeshComponent;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	// Sets default values for this actor's properties
	AProjectile();

	void HandleMovement(float DeltaTime);

	void HandlePlacement(float DeltaTime);

	void InitializeTarget(AJujutsuKaisenCharacter* InTarget = nullptr);

	void SetBehaviorType(EProjectileBehaviorType NewType);



	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return _MeshComponent;
	}

};
