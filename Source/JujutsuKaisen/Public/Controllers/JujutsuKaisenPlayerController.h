// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "JujutsuKaisenPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AJujutsuKaisenCharacter;
class UPlayerHUDWidget;
class UPlayerHealthBarWidget;
class UEnemyHealthBarWidget;

/**
 * 플레이어 컨트롤러 클래스
 * 입력 처리를 담당하고 캐릭터의 함수들을 호출하는 역할
 */
UCLASS()
class JUJUTSUKAISEN_API AJujutsuKaisenPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AJujutsuKaisenPlayerController();

	// Input Action--------------------------------------------------
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DashAction;

	/** Guard Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Q_Pressed_Action;

	/** A_Pressed Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* A_Pressed_Action;

	/** R_Pressed Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* R_Pressed_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* E_Pressed_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ER_Chord_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QR_Chord_Action;


	// Input Action Functions (래핑 함수들)
	void Move(const FInputActionValue& Value);
	void MoveCompleted();
	void Look(const FInputActionValue& Value);
	void JumpCustom();
	void StopJumping();
	void Dash();
	void StopDash();
	void Q_Pressed();
	void Q_Released();
	void A_Pressed();
	void R_Pressed();
	void R_Released();
	void E_Pressed();
	void E_Released();
	void ER_Chord();
	void QR_Chord();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void PlayerTick(float DeltaTime) override;

	/** HUD 위젯 클래스를 지정 (블루프린트에서 설정 필수) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

	/** 생성된 HUD 위젯 인스턴스 */
	UPROPERTY()
	UPlayerHUDWidget* PlayerHUDWidgetInstance = nullptr;

	/** 플레이어 캐릭터 참조 (약한 참조) */
	UPROPERTY()
	TWeakObjectPtr<AJujutsuKaisenCharacter> CachedPlayerCharacter;

	void InitializeHUD();
	void UpdateHUDData();
	void RefreshEnemyReference();
	void UpdateHUDNamesFromDataAssets();
};
