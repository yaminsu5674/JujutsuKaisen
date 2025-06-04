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

	InitHitBoxes();




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

	A_SkillAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_A_Skill.IA_A_Skill")));

	R_SkillAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Dynamic/ThirdPerson/Input/Actions/IA_R_Skill.IA_R_Skill")));


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

		// A_Skill
		EnhancedInputComponent->BindAction(A_SkillAction, ETriggerEvent::Started, this, &AJujutsuKaisenCharacter::A_Skill);

		// R_Skill
		EnhancedInputComponent->BindAction(R_SkillAction, ETriggerEvent::Started, this, &AJujutsuKaisenCharacter::R_Skill);

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


	// 캡슐 컴포넌트 높이, 반경 설정
	GetCapsuleComponent()->InitCapsuleSize(InDataAsset->GetCapsuleRadius(), InDataAsset->GetCapsuleHalfHeight());

	// Mesh 위치를 캡슐 아래로 내리기
	float HalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	SubMesh->SetRelativeLocation(FVector(0.f, 0.f, -HalfHeight));

	SubMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));


	// Attach CollisionBox to Fist, Foot
	AttachHitBoxToBone(LeftFist, InDataAsset->GetLeftFistName(), InDataAsset->GetHitBoxRadius());

	AttachHitBoxToBone(RightFist, InDataAsset->GetRightFistName(), InDataAsset->GetHitBoxRadius());

	AttachHitBoxToBone(LeftFoot, InDataAsset->GetLeftFootName(), InDataAsset->GetHitBoxRadius());

	AttachHitBoxToBone(RightFoot, InDataAsset->GetRightFootName(), InDataAsset->GetHitBoxRadius());



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

void AJujutsuKaisenCharacter::Skill()
{
	if (_AnimInstance)
	{
		_AnimInstance->SetState(ECharacterState::Skill);
	}
}

void AJujutsuKaisenCharacter::InitHitBoxes()
{
	// LeftFist
	LeftFist = CreateDefaultSubobject<UJujutsuKaisenHitBox>(TEXT("LeftFistCollision"));
	LeftFist->SetupAttachment(SubMesh);
	LeftFist->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFist->SetGenerateOverlapEvents(false);

	// RightFist
	RightFist = CreateDefaultSubobject<UJujutsuKaisenHitBox>(TEXT("RightFistCollision"));
	RightFist->SetupAttachment(SubMesh);
	RightFist->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFist->SetGenerateOverlapEvents(false);

	// LeftFoot
	LeftFoot = CreateDefaultSubobject<UJujutsuKaisenHitBox>(TEXT("LeftFootCollision"));
	LeftFoot->SetupAttachment(SubMesh);
	LeftFoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFoot->SetGenerateOverlapEvents(false);

	// RightFoot
	RightFoot = CreateDefaultSubobject<UJujutsuKaisenHitBox>(TEXT("RightFootCollision"));
	RightFoot->SetupAttachment(SubMesh);
	RightFoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFoot->SetGenerateOverlapEvents(false);
}


void AJujutsuKaisenCharacter::AttachHitBoxToBone(UJujutsuKaisenHitBox* HitBox, const FString& BoneNameStr, float Radius)
{
	UE_LOG(LogTemp, Warning, TEXT("Try bone!!!"));
	FName BoneName(*BoneNameStr);
	if (!HitBox || BoneNameStr.IsEmpty() || !SubMesh || !SubMesh->DoesSocketExist(BoneName)) return;


	UE_LOG(LogTemp, Warning, TEXT("success attach to bone"));

	HitBox->AttachToComponent(SubMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, BoneName);
	HitBox->SetSphereRadius(Radius);
	HitBox->SetRelativeLocation(FVector::ZeroVector);

	// 디버그용 구 메시 불러오기
	UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh)
	{
		// 디버그용 메시 생성
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


void AJujutsuKaisenCharacter::ActivateAttack()
{
	LeftFist->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftFist->SetGenerateOverlapEvents(true);

	RightFist->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightFist->SetGenerateOverlapEvents(true);

	LeftFoot->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftFoot->SetGenerateOverlapEvents(true);

	RightFoot->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightFoot->SetGenerateOverlapEvents(true);
}

void AJujutsuKaisenCharacter::DeactivateAttack()
{
	LeftFist->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFist->SetGenerateOverlapEvents(false);

	RightFist->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFist->SetGenerateOverlapEvents(false);

	LeftFoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFoot->SetGenerateOverlapEvents(false);

	RightFoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFoot->SetGenerateOverlapEvents(false);
}









// virtual functions
void AJujutsuKaisenCharacter::A_Skill(const FInputActionValue& Value)
{
}

void AJujutsuKaisenCharacter::R_Skill(const FInputActionValue& Value)
{
}