// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/JujutsuKaisenCharacter.h"
#include "Animations/JujutsuKaisenAnimInstance.h"
#include "Characters/CharacterStateManager.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "DataAssets/JujutsuKaisenCharacterDataAsset.h"    

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AJujutsuKaisenCharacter::AJujutsuKaisenCharacter()
{
	// 상태 매니저 초기화
	StateManager = CreateDefaultSubobject<UCharacterStateManager>(TEXT("StateManager"));

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
	GetCharacterMovement()->JumpZVelocity = DefaultJumpVelocity;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = 2.8f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 380.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AJujutsuKaisenCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

void AJujutsuKaisenCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	_AnimInstance = Cast<UJujutsuKaisenAnimInstance>(GetMesh()->GetAnimInstance());
	if (!_AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("_AnimInstance not init!!!"));
	}

	float CapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = CapsuleHalfHeight;
	SetActorLocation(NewLocation);

	SkillManager = NewObject<USkillManager>(this);
	SkillManager->RegisterOwner(this);  // 필요하면 캐릭터 정보 전달

	// Attach CollisionBox to Fist, Foot
	AttachHitBoxToBone(LeftFist, FString(TEXT("hand_l")));
	AttachHitBoxToBone(RightFist, FString(TEXT("hand_r")));
	AttachHitBoxToBone(LeftFoot, FString(TEXT("ball_l")));
	AttachHitBoxToBone(RightFoot, FString(TEXT("ball_r")));
}

void AJujutsuKaisenCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SkillManager && StateManager && StateManager->IsInState(ECharacterState::Skill))
	{
		SkillManager->TickActiveSkills(DeltaTime);
	}
	/*if (TargetCharacter)
	{
		UpdateLockOnCamera(DeltaTime);
	}*/
}


void AJujutsuKaisenCharacter::Move(const FInputActionValue& Value)
{
	// Falling 또는 Locomotion 상태일 때만 이동 가능
	if (StateManager && (StateManager->IsInState(ECharacterState::Falling) || StateManager->IsInState(ECharacterState::Locomotion)))
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
}

void AJujutsuKaisenCharacter::Look(const FInputActionValue& Value)
{
	// Falling 또는 Locomotion 상태일 때만 시점 조작 가능
	if (StateManager && (StateManager->IsInState(ECharacterState::Falling) || StateManager->IsInState(ECharacterState::Locomotion)))
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
}


void AJujutsuKaisenCharacter::Dash()
{
	if (bIsDashing)
		return;

	bIsDashing = true;
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = DashSpeed;

	// Falling 상태인지 확인 후 앞으로 대시 로직 + 애님 몽타주 재생.
	if (StateManager && StateManager->IsInState(ECharacterState::Falling))
	{
		GetCharacterMovement()->Velocity = GetActorForwardVector() * DashSpeed * 3;
		if (DashMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(DashMontage);
		}
	}
}

void AJujutsuKaisenCharacter::StopDash()
{
	if (!bIsDashing)
		return;

	bIsDashing = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = DefaultSpeed;
}

void AJujutsuKaisenCharacter::Landed(const FHitResult& Hit)
{
	// Falling 상태일 때만 착지 로직 수행
	if (StateManager && StateManager->IsInState(ECharacterState::Falling))
	{
		Super::Landed(Hit);
		JumpCount = 0;
		bDidSuperJump = false;
		bDidDoubleJump = false;
		
		// 착지 몽타주 재생
		if (LandMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(LandMontage);
		}
	}
}

void AJujutsuKaisenCharacter::Hit()
{
	if (SetState(ECharacterState::Hit))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (-1은 항상 표시)
				2.0f, // 화면에 표시되는 시간 (초)
				FColor::Red, // 색상
				TEXT("Hit!") // 메시지
			);
		}
	}
}

void AJujutsuKaisenCharacter::Die()
{
	if (SetState(ECharacterState::Dead))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (-1은 항상 표시)
				2.0f, // 화면에 표시되는 시간 (초)
				FColor::Red, // 색상
				TEXT("Dead!") // 메시지
			);
		}
	}
}

void AJujutsuKaisenCharacter::A_Pressed()
{
	if (StateManager && StateManager->SetState(ECharacterState::Skill))
	{
		if (SkillManager)
		{
			SkillManager->HandlePressed("A");
		}
	}
}

void AJujutsuKaisenCharacter::R_Pressed()
{
	if (StateManager && StateManager->SetState(ECharacterState::Skill))
	{
		if (SkillManager)
		{
			SkillManager->HandlePressed("R");
		}
	}
}

void AJujutsuKaisenCharacter::R_Released()
{
	if (StateManager && StateManager->SetState(ECharacterState::Skill))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (-1은 항상 표시)
				2.0f, // 화면에 표시되는 시간 (초)
				FColor::Green, // 색상
				TEXT("R_Released: Skill 상태 전환 성공!") // 메시지
			);
		}
		
		if (SkillManager)
		{
			SkillManager->HandleReleased("R");
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (-1은 항상 표시)
				2.0f, // 화면에 표시되는 시간 (초)
				FColor::Red, // 색상
				TEXT("R_Released: Skill 상태 전환 실패!") // 메시지
			);
		}
	}
}

// 점프 함수 (몽타주 재생 추가)
void AJujutsuKaisenCharacter::JumpCustom()
{
	// 점프 시작 시 Falling 상태로 전환 시도
	if (StateManager && StateManager->SetState(ECharacterState::Falling))
	{
		if (JumpCount == 0 && bIsDashing)
		{
			// 슈퍼 점프 (대시 중 점프)
			LaunchCharacter(FVector(0, 0, SuperJumpVelocity), false, true);
			bDidSuperJump = true;
			
			// 슈퍼 점프 몽타주 재생
			if (SuperJumpMontage && GetMesh() && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(SuperJumpMontage);
			}
		}
		else if (JumpCount == 0)
		{
			// 일반 점프
			Super::Jump(); // 기본 점프 실행
			
			// 일반 점프 몽타주 재생
			if (NormalJumpMontage && GetMesh() && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(NormalJumpMontage);
			}
		}
		else if (JumpCount == 1)
		{
			// 이단 점프
			LaunchCharacter(FVector(0, 0, DoubleJumpVelocity), false, true);
			bDidDoubleJump = true;
			
			// 이단 점프 몽타주 재생
			if (DoubleJumpMontage && GetMesh() && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(DoubleJumpMontage);
			}
		}
		JumpCount++;
		
	}
}

// 가드 함수 (몽타주 기반으로 변경)
void AJujutsuKaisenCharacter::Guard()
{
	// 가드는 액션이므로 상태 전환 없이 몽타주만 재생
	// Falling 또는 Locomotion 상태일 때만 가드 가능
	if (StateManager && (StateManager->IsInState(ECharacterState::Falling) || StateManager->IsInState(ECharacterState::Locomotion)))
	{
		bIsGuarding = true;
		
		// 가드 몽타주 재생
		if (GuardMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(GuardMontage);
		}
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (-1은 항상 표시)
				2.0f, // 화면에 표시되는 시간 (초)
				FColor::Green, // 색상
				TEXT("Guard Activated!") // 메시지
			);
		}
	}
}

// 스탑 가드 함수 (몽타주 기반으로 변경)
void AJujutsuKaisenCharacter::StopGuard()
{
	if (bIsGuarding)
	{
		bIsGuarding = false;
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (-1은 항상 표시)
				2.0f, // 화면에 표시되는 시간 (초)
				FColor::Yellow, // 색상
				TEXT("Guard Released!") // 메시지
			);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (-1은 항상 표시)
				2.0f, // 화면에 표시되는 시간 (초)
				FColor::Red, // 색상
				TEXT("Already Released!") // 메시지
			);
		}
	}
}

// 상태 관리 함수들
void AJujutsuKaisenCharacter::ForceState(ECharacterState InState)
{
	if (StateManager)
	{
		StateManager->ForceState(InState);
	}
}

bool AJujutsuKaisenCharacter::SetState(ECharacterState InState)
{
	if (StateManager)
	{
		return StateManager->SetState(InState);
	}
	return false;
}

ECharacterState AJujutsuKaisenCharacter::GetState() const
{
	if (StateManager)
	{
		return StateManager->GetCurrentState();
	}
	return ECharacterState::Locomotion;
}

// 누락된 함수들 복구
AJujutsuKaisenCharacter* AJujutsuKaisenCharacter::GetTargetCharacter() const
{
	return TargetCharacter;
}

void AJujutsuKaisenCharacter::SetTargetCharacter(AJujutsuKaisenCharacter* NewTarget)
{
	TargetCharacter = NewTarget;
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

			// 크기 조정 (스피어는 반지름 50이 기본)
			float Scale = Radius / 50.f;
			DebugMesh->SetWorldScale3D(FVector(Scale));

			// 머티리얼 설정
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

void AJujutsuKaisenCharacter::UpdateLockOnCamera(float DeltaTime)
{
	if (!CameraBoom || !TargetCharacter) return;

	FVector MyLocation = GetActorLocation();
	FVector TargetLocation = TargetCharacter->GetActorLocation();

	// 타겟 방향 계산
	FVector DirectionToTarget = (TargetLocation - MyLocation).GetSafeNormal();

	// 캐릭터 위치에서 카메라 위치로 타겟 바라보는 위치
	float DesiredDistance = 380.0f; // SpringArm 거리
	FVector DesiredCameraPosition = MyLocation - DirectionToTarget * DesiredDistance;

	// SpringArm 위치를 부드럽게 보간하여 이동
	FVector CurrentPosition = CameraBoom->GetComponentLocation();
	FVector NewPosition = FMath::VInterpTo(CurrentPosition, DesiredCameraPosition, DeltaTime, 5.0f);

	// 카메라붐의 위치 설정 (주의: 이 방법은 약간 부자연스러울 수 있음)
	CameraBoom->SetWorldLocation(NewPosition);

	// 회전도 타겟을 바라보도록
	FRotator TargetRotation = (TargetLocation - NewPosition).Rotation();
	CameraBoom->SetWorldRotation(TargetRotation);
}

// init common skills ex. guard, jump, ...
void AJujutsuKaisenCharacter::InitSkills()
{
	// 공통 스킬 초기화 로직
}

// 플레이어/AI 모드 설정
void AJujutsuKaisenCharacter::SetPlayerMode(bool bIsPlayer)
{
	bIsPlayerControlled = bIsPlayer;
	
	if (bIsPlayer)
	{
		// 플레이어 모드 설정
		UE_LOG(LogTemp, Log, TEXT("Character set to Player Mode"));
	}
	else
	{
		// AI 모드 설정
		UE_LOG(LogTemp, Log, TEXT("Character set to AI Mode"));
	}
}
