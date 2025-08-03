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

AJujutsuKaisenCharacter::AJujutsuKaisenCharacter()
{
	// My Customize settings
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Disabled;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	InitHitBoxes();
	if (bUsesWeapon)
	{
		InitWeapon();
	}

	// Set size for collision capsule
	// GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	bIsDashing = false;
	Speed = DefaultSpeed;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = DefaultSpeed;
	/*GetCharacterMovement()->MaxAcceleration = 250000.f;
	GetCharacterMovement()->BrakingFrictionFactor = 50000.f;*/
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
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

	DashAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_Dash.IA_Dash")));

	JumpAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_Jump.IA_Jump")));

	A_Pressed_Action = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_A_Pressed.IA_A_Pressed")));

	R_Pressed_Action = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_R_Pressed.IA_R_Pressed")));

	R_Released_Action = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_R_Released.IA_R_Released")));
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

		// Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AJujutsuKaisenCharacter::Dash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AJujutsuKaisenCharacter::StopDash);
		// A_Pressed
		EnhancedInputComponent->BindAction(A_Pressed_Action, ETriggerEvent::Started, this, &AJujutsuKaisenCharacter::A_Pressed);

		// R_Pressed
		EnhancedInputComponent->BindAction(R_Pressed_Action, ETriggerEvent::Started, this, &AJujutsuKaisenCharacter::R_Pressed);

		// R_Released
		EnhancedInputComponent->BindAction(R_Released_Action, ETriggerEvent::Completed, this, &AJujutsuKaisenCharacter::R_Released);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
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

	float CapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = CapsuleHalfHeight;
	SetActorLocation(NewLocation);

	SkillManager = NewObject<USkillManager>(this);
	SkillManager->RegisterOwner(this);  // 필요하면 캐릭터 참조 넘기기

	// Attach CollisionBox to Fist, Foot
	AttachHitBoxToBone(LeftFist, FString(TEXT("hand_l")));
	AttachHitBoxToBone(RightFist, FString(TEXT("hand_r")));
	AttachHitBoxToBone(LeftFoot, FString(TEXT("ball_l")));
	AttachHitBoxToBone(RightFoot, FString(TEXT("ball_r")));
}

void AJujutsuKaisenCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (_AnimInstance)
	{
		float SpeedAnimation = GetCharacterMovement()->Velocity.Size2D();
		_AnimInstance->SetSpeed(SpeedAnimation);
	}*/
	if (SkillManager)
	{
		SkillManager->TickActiveSkills(DeltaTime);
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


void AJujutsuKaisenCharacter::Dash(const FInputActionValue& Value)
{
	if (bIsDashing)
		return;

	bIsDashing = true;
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = DashSpeed;
	if (_AnimInstance)
	{
		_AnimInstance->SetState(EAnimState::Dash);
	}
}

void AJujutsuKaisenCharacter::StopDash()
{
	if (!bIsDashing)
		return;

	bIsDashing = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = DefaultSpeed;
	if (_AnimInstance)
	{
		_AnimInstance->SetState(EAnimState::Locomotion);
	}
}

void AJujutsuKaisenCharacter::JumpCustom(const FInputActionValue& Value)
{
	Super::Jump(); // 기본 동작 수행
	if (_AnimInstance)
	{
		_AnimInstance->SetState(EAnimState::Jump);
	}
}


void AJujutsuKaisenCharacter::Hit()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(EAnimState::Hit);
	}
}


void AJujutsuKaisenCharacter::Die()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(EAnimState::Dead);
	}
}

void AJujutsuKaisenCharacter::Skill()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(EAnimState::Skill);
	}
}



void AJujutsuKaisenCharacter::InitHitBoxes()
{
	// LeftFist
	LeftFist = CreateDefaultSubobject<UJujutsuKaisenHitBox>(TEXT("LeftFistCollision"));
	LeftFist->SetupAttachment(GetMesh());
	LeftFist->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftFist->SetGenerateOverlapEvents(true);

	// RightFist
	RightFist = CreateDefaultSubobject<UJujutsuKaisenHitBox>(TEXT("RightFistCollision"));
	RightFist->SetupAttachment(GetMesh());
	RightFist->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightFist->SetGenerateOverlapEvents(true);

	// LeftFoot
	LeftFoot = CreateDefaultSubobject<UJujutsuKaisenHitBox>(TEXT("LeftFootCollision"));
	LeftFoot->SetupAttachment(GetMesh());
	LeftFoot->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftFoot->SetGenerateOverlapEvents(true);

	// RightFoot
	RightFoot = CreateDefaultSubobject<UJujutsuKaisenHitBox>(TEXT("RightFootCollision"));
	RightFoot->SetupAttachment(GetMesh());
	RightFoot->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightFoot->SetGenerateOverlapEvents(true);
}

void AJujutsuKaisenCharacter::InitWeapon()
{
	//
}


void AJujutsuKaisenCharacter::AttachHitBoxToBone(UJujutsuKaisenHitBox* HitBox, const FString& BoneNameStr)
{
	UE_LOG(LogTemp, Warning, TEXT("Try bone!!!"));
	FName BoneName(*BoneNameStr);
	if (!HitBox || BoneNameStr.IsEmpty() || !GetMesh() || !GetMesh()->DoesSocketExist(BoneName)) return;


	UE_LOG(LogTemp, Warning, TEXT("success attach to bone"));

	HitBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, BoneName);
	const float Radius = 12.0f;
	HitBox->SetSphereRadius(Radius);
	HitBox->SetRelativeLocation(FVector::ZeroVector);

	if (bVisibleHitbox)
	{
		UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
		if (SphereMesh)
		{
			UStaticMeshComponent* DebugMesh = NewObject<UStaticMeshComponent>(this);
			AddInstanceComponent(DebugMesh);
			DebugMesh->RegisterComponent();
			DebugMesh->SetStaticMesh(SphereMesh);
			DebugMesh->AttachToComponent(HitBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// 메시 크기 조절 (스피어 반지름 기준으로 비례)
			float Scale = Radius / 50.f;
			/*DebugMesh->SetRelativeScale3D(FVector(Scale));*/
			DebugMesh->SetWorldScale3D(FVector(Scale));

			// 머티리얼 적용
			UMaterial* BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Engine/EngineDebugMaterials/VertexColorMaterial.VertexColorMaterial"));
			if (BaseMaterial)
			{
				UMaterialInstanceDynamic* DebugMat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
				if (DebugMat)
				{
					DebugMat->SetVectorParameterValue("Color", FLinearColor(1.f, 0.f, 0.f, 0.3f)); // 빨간색
					DebugMat->SetScalarParameterValue("Opacity", 0.3f);
					DebugMesh->SetMaterial(0, DebugMat);
				}
			}

			DebugMesh->SetVisibility(true);
			DebugMesh->SetHiddenInGame(false);
			DebugMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}


AJujutsuKaisenCharacter* AJujutsuKaisenCharacter::GetTargetCharacter() const
{
	return TargetCharacter;
}

void AJujutsuKaisenCharacter::SetTargetCharacter(AJujutsuKaisenCharacter* NewTarget)
{
	TargetCharacter = NewTarget;
}

bool AJujutsuKaisenCharacter::GetBIsDashing()
{
	return bIsDashing;
}








// virtual functions



// init common skills ex. guard, jump, ...
void AJujutsuKaisenCharacter::InitSkills()
{

}



void AJujutsuKaisenCharacter::A_Pressed(const FInputActionValue& Value)
{
	if (SkillManager)
	{
		SkillManager->HandlePressed("A");
	}
}

void AJujutsuKaisenCharacter::R_Pressed(const FInputActionValue& Value)
{
	if (SkillManager)
	{
		SkillManager->HandlePressed("R"); 
	}
}

void AJujutsuKaisenCharacter::R_Released(const FInputActionValue& Value)
{
	if (SkillManager)
	{
		SkillManager->HandleReleased("R");
	}
}


