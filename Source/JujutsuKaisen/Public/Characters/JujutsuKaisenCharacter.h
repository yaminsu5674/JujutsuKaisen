// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"
#include "Attack/JujutsuKaisenHitBox.h"
#include "Skills/SkillManager.h"
#include "Characters/CharacterStateManager.h"
#include "InputActionValue.h"
#include "JujutsuKaisenCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(Blueprintable)
class AJujutsuKaisenCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// ============================================================================
	// Constructor & Core Functions
	// ============================================================================
	AJujutsuKaisenCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void NotifyControllerChanged() override;

	// ============================================================================
	// Public Interface - Controller Access
	// ============================================================================
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dash();
	void StopDash();
	virtual void JumpCustom();
	void Landed(const FHitResult& Hit);
	virtual void Guard();
	virtual void StopGuard();
	void A_Pressed();
	void R_Pressed();
	void R_Released();

	// ============================================================================
	// State Management
	// ============================================================================
	void ForceState(ECharacterState InState);
	bool SetState(ECharacterState InState);
	ECharacterState GetState() const;
	UCharacterStateManager* GetStateManager() const { return StateManager; }

	// ============================================================================
	// Character Actions
	// ============================================================================
	void Hit();
	void Die();
	virtual void InitSkills();

	// ============================================================================
	// Target Management
	// ============================================================================
	AJujutsuKaisenCharacter* GetTargetCharacter() const;
	void SetTargetCharacter(AJujutsuKaisenCharacter* NewTarget);

	// ============================================================================
	// Getters & Setters
	// ============================================================================
	bool GetBIsDashing() const { return bIsDashing; }
	uint8 GetJumpCount() const { return JumpCount; }
	bool GetBDidSuperJump() const { return bDidSuperJump; }
	void SetBDidSuperJump(bool bValue) { bDidSuperJump = bValue; }
	bool GetBDidDoubleJump() const { return bDidDoubleJump; }
	void SetBDidDoubleJump(bool bValue) { bDidDoubleJump = bValue; }

	// ============================================================================
	// Camera Access
	// ============================================================================
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// ============================================================================
	// Character Mode
	// ============================================================================
	UFUNCTION(BlueprintCallable, Category = "Character Mode")
	void SetPlayerMode(bool bIsPlayer);

	// ============================================================================
	// Gravity Control
	// ============================================================================
	void SetGravityEnabled(bool bEnabled);

	// ============================================================================
	// Movement Control
	// ============================================================================
	void SetCanMove(bool bCanMove);
	bool GetCanMove() const { return bCanMove; }

protected:
	// ============================================================================
	// Protected Virtual Functions
	// ============================================================================
	virtual void InitWeapon();
	virtual void UpdateCameraMovement(float DeltaTime);

	// ============================================================================
	// Protected Helper Functions
	// ============================================================================
	void InitHitBoxes();
	void AttachHitBoxToBone(UJujutsuKaisenHitBox* HitBox, const FString& BoneNameStr);

	// ============================================================================
	// Core Components
	// ============================================================================
	UPROPERTY()
	UCharacterStateManager* StateManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Skill")
	USkillManager* SkillManager;

	// ============================================================================
	// Camera Components
	// ============================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// ============================================================================
	// Camera Settings
	// ============================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings", meta = (AllowPrivateAccess = "true"))
	float SpringArmLength = 380.0f;

	// ============================================================================
	// Character Components
	// ============================================================================
	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* LeftFist;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* RightFist;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* LeftFoot;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* RightFoot;

	// ============================================================================
	// Character Parameters
	// ============================================================================
	UPROPERTY(EditAnywhere, Category = "Character params")
	bool bVisibleHitbox = false;

	UPROPERTY(EditAnywhere, Category = "Character params")
	bool bUsesWeapon = false;

	UPROPERTY(EditAnywhere, Category = "Character components")
	AActor* Weapon;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float MaxHealth = 1000;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY()
	bool bCanMove = true;

	// ============================================================================
	// Movement Parameters
	// ============================================================================
	UPROPERTY(VisibleAnywhere)
	uint8 JumpCount = 0;

	UPROPERTY(VisibleAnywhere)
	bool bDidSuperJump = false;

	UPROPERTY(VisibleAnywhere)
	bool bDidDoubleJump = false;

	UPROPERTY(VisibleAnywhere)
	bool bIsDashing = false;

	UPROPERTY(VisibleAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float DefaultSpeed = 500;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float DashSpeed = 1200;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float DefaultJumpVelocity = 1800;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float DoubleJumpVelocity = 2400;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float SuperJumpVelocity = 2400;

	// ============================================================================
	// Animation System
	// ============================================================================
	UPROPERTY()
	class UJujutsuKaisenAnimInstance* _AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* NormalJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* SuperJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* DoubleJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* GuardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* LandMontage;

	// ============================================================================
	// Character State
	// ============================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character params", meta = (AllowPrivateAccess = "true"))
	bool bIsGuarding = false;

	UPROPERTY(BlueprintReadOnly, Category = "Character Mode")
	bool bIsPlayerControlled = true;

protected:
	// ============================================================================
	// Target Members
	// ============================================================================
	UPROPERTY()
	AJujutsuKaisenCharacter* TargetCharacter = nullptr;
};