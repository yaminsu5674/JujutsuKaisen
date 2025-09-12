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


	// 키 상태 추적 변수들
	UPROPERTY()
	bool bEKeyPressed;

	UPROPERTY()
	bool bRKeyPressed;

	UPROPERTY()
	bool bQKeyPressed;

	// 콤보 실행 상태 추적 변수들
	UPROPERTY()
	bool bERComboExecuted;

	UPROPERTY()
	bool bQRComboExecuted;

	// 콤보 시스템을 위한 타이머 핸들들
	UPROPERTY()
	FTimerHandle EKeyTimer;

	UPROPERTY()
	FTimerHandle QKeyTimer;

	UPROPERTY()
	FTimerHandle RKeyTimer;

	UPROPERTY()
	FTimerHandle JujutsuComboTimer;

	// 콤보 딜레이 (100ms 안에 두 키 같이 눌리면 콤보로 처리)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	float JujutsuComboDelay = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	float KeyReleaseDelay = 0.2f;

	// Input Action Functions (래핑 함수들)
	void Move(const FInputActionValue& Value);
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

	// 키 상태 관리 함수들
	void JudgeJujutsuCombo();
	void StartJujutsuComboTimer();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
};
