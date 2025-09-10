// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"
#include "Attack/JujutsuKaisenHitBox.h"
#include "Skills/SkillManager.h"
#include "Characters/CharacterStateManager.h"
#include "JujutsuKaisenCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(Blueprintable)
class AJujutsuKaisenCharacter : public ACharacter
{
	GENERATED_BODY()

	// 상태 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	UCharacterStateManager* StateManager;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// Input Action--------------------------------------------------

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// Dash
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	// Guard
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GuardAction;

	// Combo Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* A_Pressed_Action;

	// Active Skill

	// Main Skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* R_Pressed_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* R_Released_Action;

	// Ultimate Skill



	// Character Components --------------------------------------------------------

	/*UPROPERTY(EditAnywhere,  Category = "Character components")
	USkeletalMeshComponent* SubMesh;*/

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* LeftFist;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* RightFist;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* LeftFoot;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* RightFoot;

	// Character Params ---------------------------------------------------------
	UPROPERTY()
	AJujutsuKaisenCharacter* TargetCharacter = nullptr;

protected:
	UPROPERTY(EditAnywhere, Category = "Character params")
	bool bVisibleHitbox = false;

	UPROPERTY()
	USkillManager* SkillManager;

	UPROPERTY(EditAnywhere, Category = "Character params")
	bool bUsesWeapon = false;

	UPROPERTY(EditAnywhere, Category = "Character components")
	AActor* Weapon;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float MaxHealth = 1000;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(VisibleAnywhere)
	uint8 JumpCount = 0;

	// Header (.h)
	UPROPERTY(VisibleAnywhere)
	bool bDidSuperJump = false;

	UPROPERTY(VisibleAnywhere)
	bool bDidDoubleJump = false;


	UPROPERTY(VisibleAnywhere)
	bool bIsDashing = false;

	// Speed . It should be controlled on Character Not AnimationInstance
	UPROPERTY(VisibleAnywhere)
	float Speed;

	// Speed . It should be controlled on Character Not AnimationInstance
	UPROPERTY(EditAnywhere, Category = "Character params")
	float DefaultSpeed = 500;

	// Speed . It should be controlled on Character Not AnimationInstance
	UPROPERTY(EditAnywhere, Category = "Character params")
	float DashSpeed = 1200;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float DefaultJumpVelocity = 1800;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float DoubleJumpVelocity = 2400;

	UPROPERTY(EditAnywhere, Category = "Character params")
	float SuperJumpVelocity = 2400;

	// AnimInstance
	UPROPERTY()
	class UJujutsuKaisenAnimInstance* _AnimInstance;

	// 애니메이션 몽타주들 (블루프린트에서 초기화)
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

	// 가드 상태 플래그
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character params", meta = (AllowPrivateAccess = "true"))
	bool bIsGuarding = false;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Dash
	void Dash();

	void StopDash();

	virtual void JumpCustom();

	void Landed(const FHitResult& Hit);

	virtual void Guard();

	virtual void StopGuard();

	void A_Pressed();

	void R_Pressed();

	void R_Released();

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	void InitHitBoxes();

	virtual void InitWeapon();

	void AttachHitBoxToBone(UJujutsuKaisenHitBox* HitBox, const FString& BoneNameStr);

	virtual void UpdateLockOnCamera(float DeltaTime);

public:
	AJujutsuKaisenCharacter();

	virtual void Tick(float DeltaTime) override;

	void ForceState(ECharacterState InState);

	bool SetState(ECharacterState InState);

	ECharacterState GetState() const;

	// 상태 매니저 접근
	UCharacterStateManager* GetStateManager() const { return StateManager; }

	virtual void InitSkills();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	AJujutsuKaisenCharacter* GetTargetCharacter () const;

	void SetTargetCharacter(AJujutsuKaisenCharacter* NewTarget);



	bool GetBIsDashing() const { return bIsDashing; }
	uint8 GetJumpCount() const { return JumpCount; }
	bool GetBDidSuperJump() const { return bDidSuperJump; }
	void SetBDidSuperJump(bool bValue) { bDidSuperJump = bValue; }
	bool GetBDidDoubleJump() const { return bDidDoubleJump; }
	void SetBDidDoubleJump(bool bValue) { bDidDoubleJump = bValue; }

	
	void Hit();

	void Die();

	// 플레이어/AI 모드 구분
	UPROPERTY(BlueprintReadOnly, Category = "Character Mode")
	bool bIsPlayerControlled = true;

	// AI용 래핑 함수들 (인자 없음)
	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_Move();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_Look();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_JumpCustom();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_Dash();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_StopDash();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_Guard();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_StopGuard();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_A_Pressed();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_R_Pressed();

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	void AI_R_Released();

	// 모드 설정
	UFUNCTION(BlueprintCallable, Category = "Character Mode")
	void SetPlayerMode(bool bIsPlayer);
};

