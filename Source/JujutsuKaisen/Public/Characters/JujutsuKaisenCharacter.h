// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"
#include "Attack/JujutsuKaisenHitBox.h"
#include "Skills/SkillManager.h"
#include "JujutsuKaisenCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Locomotion = 0,
	Dash = 1,
	Jump = 2,
	SuperJump = 3,
	Skill = 4,
	Hit = 5,
	Dead = 6
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(Blueprintable)
class AJujutsuKaisenCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY()
	ECharacterState CurrentState = ECharacterState::Locomotion;

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

	// AnimInstance
	UPROPERTY()
	class UJujutsuKaisenAnimInstance* _AnimInstance;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Dash
	void Dash(const FInputActionValue& Value);

	void StopDash();

	void JumpCustom(const FInputActionValue& Value);

	void A_Pressed(const FInputActionValue& Value);

	void R_Pressed(const FInputActionValue& Value);

	void R_Released(const FInputActionValue& Value);

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	void InitHitBoxes();

	virtual void InitWeapon();

	void AttachHitBoxToBone(UJujutsuKaisenHitBox* HitBox, const FString& BoneNameStr);



public:
	AJujutsuKaisenCharacter();

	virtual void Tick(float DeltaTime) override;

	void SetState(ECharacterState InState);

	virtual void InitSkills();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	AJujutsuKaisenCharacter* GetTargetCharacter () const;

	void SetTargetCharacter(AJujutsuKaisenCharacter* NewTarget);

	bool GetBIsDashing();
	
	void Hit();

	void Die();

	void Skill();
};

