// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"
#include "Attack/JujutsuKaisenHitBox.h"
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

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* A_SkillAction;




	// Character Components --------------------------------------------------------

	UPROPERTY(EditAnywhere,  Category = "Character components")
	USkeletalMeshComponent* SubMesh;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* LeftFist;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* RightFist;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* LeftFoot;

	UPROPERTY(VisibleAnywhere, Category = "Character components")
	UJujutsuKaisenHitBox* RightFoot;




	// Character Params ---------------------------------------------------------

	UPROPERTY(EditAnywhere, Category = "Character params")
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

public:
	class UJujutsuKaisenAnimInstance* _AnimInstance;

public:
	AJujutsuKaisenCharacter();

	virtual void Tick(float DeltaTime) override;


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void JumpCustom(const FInputActionValue& Value);

	virtual void A_Skill(const FInputActionValue& Value);

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	void InitHitBoxes();

	void AttachHitBoxToBone(UJujutsuKaisenHitBox* HitBox, const FString& BoneNameStr, float Radius);



public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// call this function when game mode makes character instance;
	void InitCharacterWithData(UJujutsuKaisenCharacterDataAsset* InDataAsset);

	void Hit();

	void Die();

	void Skill();

	void ActivateAttack();

	void DeactivateAttack();
};

