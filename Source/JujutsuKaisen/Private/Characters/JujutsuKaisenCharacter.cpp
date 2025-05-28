// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/JujutsuKaisenCharacter.h"
#include "Animations/JujutsuKaisenAnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"
#include "InputMappingContext.h"    

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AJujutsuKaisenCharacter

AJujutsuKaisenCharacter::AJujutsuKaisenCharacter()
{

	// My Customize settings
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Disabled;

	SubMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubMesh"));
	SubMesh->SetupAttachment(GetMesh());
	SubMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	GetMesh()->SetVisibility(false);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
	FSoftObjectPath DataAssetPath(TEXT("/Game/Dynamic/DataAsset/Mixamo.Mixamo"));
	UJujutsuKaisenCharacterDataAsset* MixamoAsset = Cast<UJujutsuKaisenCharacterDataAsset>(DataAssetPath.TryLoad());
	if (MixamoAsset)
	{
		GetMesh()->SetSkeletalMesh(MixamoAsset->GetMesh());
		GetMesh()->SetAnimInstanceClass(MixamoAsset->GetAnimBP());
	}




	/*this->JumpMaxCount = 2;*/


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	// Input Mapping Actions below
	DefaultMappingContext = Cast<UInputMappingContext>(StaticLoadObject(UInputMappingContext::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/IMC_Default.IMC_Default.IMC_Default")));

	MoveAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_Move.IA_Move")));

	LookAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_Look.IA_Look")));

	JumpAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_Jump.IA_Jump")));




}

//////////////////////////////////////////////////////////////////////////
// Input

void AJujutsuKaisenCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AJujutsuKaisenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AJujutsuKaisenCharacter::JumpCustom);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJujutsuKaisenCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJujutsuKaisenCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}




void AJujutsuKaisenCharacter::InitCharacterWithData(UJujutsuKaisenCharacterDataAsset* InDataAsset)
{
	if (!InDataAsset) return;

	// Skeletal Mesh 설정
	if (InDataAsset->GetMesh())
	{
		SubMesh->SetSkeletalMesh(InDataAsset->GetMesh());
	}

	// AnimBP 설정
	if (InDataAsset->GetAnimBP())
	{
		SubMesh->SetAnimInstanceClass(InDataAsset->GetAnimBP());

	}

	// Mesh 스케일 설정
	SubMesh->SetRelativeScale3D(FVector(InDataAsset->GetMeshScale()));

	// Mesh 위치를 캡슐 아래로 내리기
	float HalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	SubMesh->SetRelativeLocation(FVector(0.f, 0.f, -HalfHeight));

	SubMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

}



void AJujutsuKaisenCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	_AnimInstance = Cast<UJujutsuKaisenAnimInstance>(GetMesh()->GetAnimInstance());
	if (_AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("_AnimInstance init!!!"));
	}
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));	
	// }
}

void AJujutsuKaisenCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_AnimInstance)
	{
		float Speed = GetCharacterMovement()->Velocity.Size2D();
		_AnimInstance->SetSpeed(Speed);
	}
	

}

void AJujutsuKaisenCharacter::Move(const FInputActionValue& Value)
{

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AJujutsuKaisenCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AJujutsuKaisenCharacter::JumpCustom(const FInputActionValue& Value)
{
	Super::Jump(); // 기본 동작 수행
	if (_AnimInstance)
	{
		_AnimInstance->SetState(ECharacterState::Jump);
	}
	 /*if (GEngine)
	 {
	 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Jumping!"));	
	 }*/
}



void AJujutsuKaisenCharacter::Hit()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(ECharacterState::Hit);
	}
}


void AJujutsuKaisenCharacter::Die()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(ECharacterState::Dead);
	}
}

void AJujutsuKaisenCharacter::Skill_1()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(ECharacterState::Skill_1);
	}
}

void AJujutsuKaisenCharacter::Skill_2()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(ECharacterState::Skill_2);
	}
}

void AJujutsuKaisenCharacter::Skill_3()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(ECharacterState::Skill_3);
	}
}