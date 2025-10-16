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

// ============================================================================
// Constructor & Core Functions
// ============================================================================

AJujutsuKaisenCharacter::AJujutsuKaisenCharacter()
{
	// 상태 매니저 초기화
	StateManager = CreateDefaultSubobject<UCharacterStateManager>(TEXT("StateManager"));
	if (StateManager)
	{
		StateManager->SetOwnerCharacter(this);
		UE_LOG(LogTemp, Log, TEXT("Constructor: StateManager created successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Constructor: StateManager creation failed!"));
	}

	// 기본 설정
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Disabled;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// 스킬 상태 배열 초기화 (Q, E, R, A, S, D, ER, QR 순서)
	bIsUsingSkills.Init(false, 8);


	// 컴포넌트 초기화
	InitHitBoxes();
	if (bUsesWeapon)
	{
		InitWeapon();
	}

	// 회전 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 이동 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	bIsDashing = false;
	Speed = DefaultSpeed;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->JumpZVelocity = DefaultJumpVelocity;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = DefaultGravityScale;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 카메라 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = SpringArmLength;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetAbsolute(false, true, false);
	

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AJujutsuKaisenCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 기본값 설정
	Health = MaxHealth;
	bCanMove = true; // 게임 시작 시 이동 가능하도록 강제 설정
	
	// StateManager 초기 상태 설정
	if (StateManager)
	{
		StateManager->ForceState(ECharacterState::Locomotion);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay: StateManager is NULL!"));
		UE_LOG(LogTemp, Error, TEXT("Character: %s"), *GetName());
		UE_LOG(LogTemp, Error, TEXT("Class: %s"), *GetClass()->GetName());
	}

	_AnimInstance = Cast<UJujutsuKaisenAnimInstance>(GetMesh()->GetAnimInstance());
	if (!_AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("_AnimInstance not init!!!"));
	}

	// 위치 조정
	float CapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = CapsuleHalfHeight;
	SetActorLocation(NewLocation);


	// 히트박스 부착
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
	
	
	// 플레이어가 제어하는 캐릭터만 카메라 무브 업데이트
	if (bIsPlayerControlled)
	{
		UpdateCameraMovement(DeltaTime);
	}
}

void AJujutsuKaisenCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

// ============================================================================
// Public Interface - Controller Access
// ============================================================================

void AJujutsuKaisenCharacter::Move(const FInputActionValue& Value)
{
	if (!GetCanMove())
	{
		return;
	}

	// Falling 또는 Locomotion 상태일 때만 이동 가능
	if (StateManager && (StateManager->IsInState(ECharacterState::Falling) || StateManager->IsInState(ECharacterState::Locomotion)))
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr && CameraBoom != nullptr)
		{
			// 카메라 회전을 기준으로 이동 방향 계산
			const FRotator CameraRotation = CameraBoom->GetComponentRotation();
			const FRotator YawRotation(0, CameraRotation.Yaw, 0);

			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

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
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
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

	// Falling 상태인지 확인 후 앞으로 대시 로직 + 애님 몽타주 재생
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
			
			if (SuperJumpMontage && GetMesh() && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(SuperJumpMontage);
			}
		}
		else if (JumpCount == 0)
		{
			// 일반 점프
			Super::Jump();
			
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
			
			if (DoubleJumpMontage && GetMesh() && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(DoubleJumpMontage);
			}
		}
		JumpCount++;
	}
}

void AJujutsuKaisenCharacter::Landed(const FHitResult& Hit)
	{
		Super::Landed(Hit);
		JumpCount = 0;
		bDidSuperJump = false;
		bDidDoubleJump = false;
	// Falling 상태일 때만 착지 로직 수행
	if (StateManager && StateManager->IsInState(ECharacterState::Falling))
	{
		SetCanMove(false);	
		// 착지 몽타주 재생
		if (LandMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(LandMontage);
		}
	}
}

void AJujutsuKaisenCharacter::Guard()
{
	// 다른 스킬이 사용 중인지 확인
	if (IsOtherSkillInUse(ESkillIndex::Q))
	{
		return; // 다른 스킬 사용 중이면 실행하지 않음
	}

	// Q 스킬 사용 시작
	SetSkillInUse(ESkillIndex::Q, true);
	
	if (StateManager && StateManager->SetState(ECharacterState::Skill))
	{
		if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Q_Pressed Called!"));
			}
		if (StateManager && (StateManager->IsInState(ECharacterState::Falling) || StateManager->IsInState(ECharacterState::Locomotion)))
		{
			bIsGuarding = true;
			
			if (GuardMontage && GetMesh() && GetMesh()->GetAnimInstance())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(GuardMontage);
			}
		}
	}
}

void AJujutsuKaisenCharacter::StopGuard()
{
	// Q 스킬이 사용 중일 때만 Released 처리
	if (IsSkillInUse(ESkillIndex::Q))
	{
		if (StateManager && StateManager->IsInState(ECharacterState::Skill))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Q_Released Called!"));
			}
			if (bIsGuarding)
				{
					bIsGuarding = false;

				}
		}
	}
}

void AJujutsuKaisenCharacter::A_Pressed()
{
	// 다른 스킬이 사용 중인지 확인
	if (IsOtherSkillInUse(ESkillIndex::A))
	{
		return; // 다른 스킬 사용 중이면 실행하지 않음
	}

	// A 스킬 사용 시작
	SetSkillInUse(ESkillIndex::A, true);
	
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
	// 다른 스킬이 사용 중인지 확인
	if (IsOtherSkillInUse(ESkillIndex::R))
	{
		return; // 다른 스킬 사용 중이면 실행하지 않음
	}

	// R 스킬 사용 시작
	SetSkillInUse(ESkillIndex::R, true);
	
	if (StateManager && StateManager->SetState(ECharacterState::Skill))
	{
		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("R_Pressed Called!"));
		// }
		if (SkillManager)
		{
			SkillManager->HandlePressed("R");
		}
	}
}

void AJujutsuKaisenCharacter::R_Released()
{
	// R 스킬이 사용 중일 때만 Released 처리
	if (IsSkillInUse(ESkillIndex::R))
	{
		if (StateManager && StateManager->IsInState(ECharacterState::Skill))
		{
			// if (GEngine)
			// 	{
			// 		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("R_Released Called!"));
			// 	}

			if (SkillManager)
			{
				SkillManager->HandleReleased("R");
			}
		}
	}
	
}

void AJujutsuKaisenCharacter::E_Pressed()
{
	// 다른 스킬이 사용 중인지 확인
	if (IsOtherSkillInUse(ESkillIndex::E))
	{
		return; // 다른 스킬 사용 중이면 실행하지 않음
	}

	// E 스킬 사용 시작
	SetSkillInUse(ESkillIndex::E, true);
	
	if (StateManager && StateManager->SetState(ECharacterState::Skill))
	{
		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("E_Pressed Called!"));
		// }
		if (SkillManager)
		{
			SkillManager->HandlePressed("E");
		}
	}
}


void AJujutsuKaisenCharacter::QR_Pressed()
{
	// 다른 스킬이 사용 중인지 확인
	if (IsOtherSkillInUse(ESkillIndex::QR))
	{
		return; // 다른 스킬 사용 중이면 실행하지 않음
	}

	// QR 콤보 스킬 사용 시작
	SetSkillInUse(ESkillIndex::QR, true);
	
	if (StateManager && StateManager->SetState(ECharacterState::Skill))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("QR_Pressed Called!"));
		}
		if (SkillManager)
		{
			SkillManager->HandlePressed("QR");
		}
	}
}

void AJujutsuKaisenCharacter::ER_Pressed()
{
	// 다른 스킬이 사용 중인지 확인
	if (IsOtherSkillInUse(ESkillIndex::ER))
	{
		return; // 다른 스킬 사용 중이면 실행하지 않음
	}

	// ER 콤보 스킬 사용 시작
	SetSkillInUse(ESkillIndex::ER, true);
	
	if (StateManager && StateManager->SetState(ECharacterState::Skill))
	{
		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ER_Pressed Called!"));
		// }
		if (SkillManager)
		{
			SkillManager->HandlePressed("ER");
		}
	}
}

// ============================================================================
// State Management
// ============================================================================

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

// ============================================================================
// Character Actions
// ============================================================================

void AJujutsuKaisenCharacter::Hit()
{
	if (StateManager && StateManager->IsInState(ECharacterState::Hit))
	{
		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("Hit!"));
		// }
	}
}

void AJujutsuKaisenCharacter::Die()
{
	if (SetState(ECharacterState::Dead))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Dead!"));
		}
	}
}


// ============================================================================
// Target Management
// ============================================================================

AJujutsuKaisenCharacter* AJujutsuKaisenCharacter::GetTargetCharacter() const
{
	return TargetCharacter;
}

void AJujutsuKaisenCharacter::SetTargetCharacter(AJujutsuKaisenCharacter* NewTarget)
{
	TargetCharacter = NewTarget;
}

// ============================================================================
// Character Mode
// ============================================================================

void AJujutsuKaisenCharacter::SetPlayerMode(bool bIsPlayer)
{
	bIsPlayerControlled = bIsPlayer;
}

// ============================================================================
// Gravity Control
// ============================================================================

void AJujutsuKaisenCharacter::SetGravityEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		GetCharacterMovement()->GravityScale = DefaultGravityScale; // 기본값 사용
	}
	else
	{
		// 중력 끄기 + 수직 속도도 0으로 만들어서 공중에서 멈춤
		GetCharacterMovement()->GravityScale = 0.0f;
		FVector CurrentVelocity = GetCharacterMovement()->Velocity;
		CurrentVelocity.Z = 0.0f; // 수직 속도만 0으로
		GetCharacterMovement()->Velocity = CurrentVelocity;
	}
}

// ============================================================================
// Movement Control
// ============================================================================

void AJujutsuKaisenCharacter::SetCanMove(bool bCanMoveValue)
{
	bCanMove = bCanMoveValue;
}

// ============================================================================
// Protected Helper Functions
// ============================================================================

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
	// 무기 초기화 로직
}

void AJujutsuKaisenCharacter::AttachHitBoxToBone(UJujutsuKaisenHitBox* HitBox, const FString& BoneNameStr)
{
	FName BoneName(*BoneNameStr);
	if (!HitBox || BoneNameStr.IsEmpty() || !GetMesh() || !GetMesh()->DoesSocketExist(BoneName)) return;

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

void AJujutsuKaisenCharacter::UpdateCameraMovement(float DeltaTime)
{
	// 플레이어가 제어하는 캐릭터만 카메라 무브먼트 실행
	if (!bIsPlayerControlled || !CameraBoom || !TargetCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateCameraMovement: bIsPlayerControlled is FALSE or CameraBoom or TargetCharacter is NULL!"));
		return;
	}
	
	FVector MyLocation = CameraBoom->GetComponentLocation();
    FVector TargetLocation = TargetCharacter->GetActorLocation();

    // 타겟을 바라보는 전체 회전 계산 (Pitch 포함)
    FRotator TargetRotation = (TargetLocation - MyLocation).Rotation();
    
    // Z축 기준으로 시계방향으로 10도 추가 회전
    TargetRotation.Yaw -= 10.0f;

    // 보간 회전
    FRotator CurrentRotation = CameraBoom->GetComponentRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);

     // 스프링암은 항상 코드로만 회전 제어
     CameraBoom->bUsePawnControlRotation = false;
     CameraBoom->SetWorldRotation(NewRotation);

     // 길이는 고정
     CameraBoom->TargetArmLength = SpringArmLength;
	
}

void AJujutsuKaisenCharacter::ResetSkillVariables()
{
	// 모든 스킬 상태를 false로 리셋
	for (int32 i = 0; i < bIsUsingSkills.Num(); i++)
	{
		bIsUsingSkills[i] = false;
	}
}

// ============================================================================
// 스킬 상태 관리 헬퍼 함수들
// ============================================================================

bool AJujutsuKaisenCharacter::IsOtherSkillInUse(ESkillIndex CurrentSkillIndex) const
{
	uint8 CurrentIndex = static_cast<uint8>(CurrentSkillIndex);
	
	// 현재 스킬을 제외한 다른 스킬들이 사용 중인지 확인
	for (int32 i = 0; i < bIsUsingSkills.Num(); i++)
	{
		if (i != CurrentIndex && bIsUsingSkills[i])
		{
			return true; // 다른 스킬이 사용 중
		}
	}
	return false; // 다른 스킬이 사용 중이 아님
}

void AJujutsuKaisenCharacter::SetSkillInUse(ESkillIndex SkillIndex, bool bInUse)
{
	uint8 Index = static_cast<uint8>(SkillIndex);
	if (Index < bIsUsingSkills.Num())
	{
		bIsUsingSkills[Index] = bInUse;
	}
}

bool AJujutsuKaisenCharacter::IsSkillInUse(ESkillIndex SkillIndex) const
{
	uint8 Index = static_cast<uint8>(SkillIndex);
	if (Index < bIsUsingSkills.Num())
	{
		return bIsUsingSkills[Index];
	}
	return false;
}
