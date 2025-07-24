// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/Gojo_Satoru/Aka.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

UAka::UAka()
{
	bWantsTick = true;
	AkaProjectile = nullptr;
	state = 0;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EarlyMontageFinder(TEXT("AnimMontage'/Game/Static/Animation_Source/Gojo_Satoru/Aka/AkaEarly_Montage.AkaEarly_Montage'"));
	if (EarlyMontageFinder.Succeeded()) AkaEarlyMontage = EarlyMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> LateMontageFinder(TEXT("AnimMontage'/Game/Static/Animation_Source/Gojo_Satoru/Aka/AkaLate_Montage.AkaLate_Montage'"));
	if (LateMontageFinder.Succeeded()) AkaLateMontage = LateMontageFinder.Object;

	static ConstructorHelpers::FClassFinder<AProjectile> ProjectileBPClass(TEXT("/Game/Dynamic/Projectiles/BP_Projectile_Aka"));
	if (ProjectileBPClass.Succeeded()) ProjectileClass = ProjectileBPClass.Class;
}

void UAka::TickSkill(float DeltaTime)
{
	// 충전 등 지속 로직
}

void UAka::OnPressed(AJujutsuKaisenCharacter* Target)
{
	state = 1;

	if (AnimInstance && AkaLateMontage)
	{
		BindMontageNotifies();
		AnimInstance->Montage_Play(AkaLateMontage);
	}

	if (!AkaProjectile)
	{
		SpawnProjectile();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Aka projectile already exists!"));
	}
}

void UAka::OnReleased(AJujutsuKaisenCharacter* Target)
{
	if (state == 1)
	{
		AnimInstance->Montage_Stop(0.2f);

		if (AnimInstance && AkaEarlyMontage)
		{
			AnimInstance->Montage_Play(AkaEarlyMontage);
		}
		return;
	}

	if (state == 2)
	{
		UnbindMontageNotifies();
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify3Begin);
		AnimInstance->Montage_Resume(AkaLateMontage);

		LaunchProjectile();
	}
}

void UAka::SpawnProjectile()
{
	if (!ProjectileClass || !Owner) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;

	const FVector Offset = Owner->GetActorForwardVector() * 50.f;
	const FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(FName("hand_L")) + Offset;
	const FRotator SpawnRotation = Owner->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner;

	AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (!Projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Aka projectile"));
		return;
	}

	Projectile->InitializeParams();
	AkaProjectile = Projectile;
}

void UAka::LaunchProjectile()
{
	if (AkaProjectile)
	{
		AkaProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AkaProjectile->SetBehaviorType(EProjectileBehaviorType::Move);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Aka Released!"));
		}

		AkaProjectile = nullptr;
	}
}

// 초기화 함수
void UAka::ResetSkill()
{
	state = 0;

	if (AkaProjectile)
	{
		//AkaProjectile->Destroy();
		AkaProjectile = nullptr;
	}

	UnbindMontageNotifies();
}

// 노티파이 바인딩
void UAka::BindMontageNotifies()
{
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify2Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify3Begin);

		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify2Begin);
	}
}

// 노티파이 언바인딩
void UAka::UnbindMontageNotifies()
{
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify1Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify2Begin);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify3Begin);
	}
}

void UAka::OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("AkaNotify1"))
	{
		LaunchProjectile();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Good!"));
		}
	}
}

void UAka::OnMontageNotify2Begin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("AkaNotify2"))
	{
		state = 2;
		AnimInstance->Montage_Pause(AkaLateMontage);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Good!"));
		}
	}
}

void UAka::OnMontageNotify3Begin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("AkaNotify3"))
	{
		UE_LOG(LogTemp, Log, TEXT("Montage Notify 3 Triggered"));
	}
}




//#include "Skills/Gojo_Satoru/Aka.h"
//#include "Animation/AnimInstance.h"
//#include "Animation/AnimMontage.h"
////#include "Animation/AnimNotifies/AnimNotifyQueue.h"
//
//UAka::UAka()
//{
//	bWantsTick = true;
//	AkaProjectile = nullptr;
//	state = 0;
//
//	static ConstructorHelpers::FObjectFinder<UAnimMontage> EarlyMontageFinder(TEXT("AnimMontage'/Game/Static/Animation_Source/Gojo_Satoru/Aka/AkaEarly_Montage.AkaEarly_Montage'"));
//	if (EarlyMontageFinder.Succeeded())
//	{
//		AkaEarlyMontage = EarlyMontageFinder.Object;
//	}
//
//	static ConstructorHelpers::FObjectFinder<UAnimMontage> LateMontageFinder(TEXT("AnimMontage'/Game/Static/Animation_Source/Gojo_Satoru/Aka/AkaLate_Montage.AkaLate_Montage'"));
//	if (LateMontageFinder.Succeeded())
//	{
//		AkaLateMontage = LateMontageFinder.Object;
//	}
//
//	static ConstructorHelpers::FClassFinder<AProjectile> ProjectileBPClass(TEXT("/Game/Dynamic/Projectiles/BP_Projectile_Aka")); 
//	if (ProjectileBPClass.Succeeded())
//	{
//		ProjectileClass = ProjectileBPClass.Class;
//	}
//}
//
//
//void UAka::TickSkill(float DeltaTime)
//{
//	// Charging logic & effect
//}
//
//
//void UAka::OnPressed(AJujutsuKaisenCharacter* Target)
//{
//	state = 1;
//	if (AnimInstance && AkaLateMontage)
//	{
//		// 노티파이 델리게이트 바인딩
//		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify2Begin);
//
//		AnimInstance->Montage_Play(AkaLateMontage);
//	}
//	// 발사체가 이미 존재하면 새로 생성하지 않음
//	if (AkaProjectile)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Aka projectile already exists!"));
//		return;
//	}
//
//	UWorld* World = Owner->GetWorld();
//	if (!World) return;
//
//	// 스폰 위치 계산
//	FVector ForwardOffset = Owner->GetActorForwardVector() * 50.f;
//	FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(FName("hand_L")) + ForwardOffset;
//	FRotator SpawnRotation = Owner->GetActorRotation();
//
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.Owner = Owner;
//	SpawnParams.Instigator = Owner;
//
//	// 발사체 생성
//	AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
//	if (!Projectile)
//	{
//		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Aka projectile"));
//		return;
//	}
//	//FName SocketName = FName("hand_L");
//	Projectile->InitializeParams();
//	/*Projectile->AttachToComponent(
//		Owner->GetMesh(),
//		FAttachmentTransformRules::KeepRelativeTransform,
//		SocketName
//	);*/
//
//	AkaProjectile = Projectile; // 참조 저장
//	
//}
//
//void UAka::OnReleased(AJujutsuKaisenCharacter* Target)
//{
//	if (state == 1)
//	{
//		AnimInstance->Montage_Stop(0.2f);
//		if (AnimInstance && AkaEarlyMontage)
//		{
//			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify1Begin);
//			AnimInstance->Montage_Play(AkaEarlyMontage);
//		}
//
//		return;
//	}
//	if (state == 2)
//	{
//		// 여기서 다시 중단됐던 몽타주를 "AkaNotify3"이름을 가진 노티파이지점 근처부터 다시하고싶어.
//		// 1. 노티파이 바인딩 중복 방지
//		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAka::OnMontageNotify3Begin);
//		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAka::OnMontageNotify3Begin);
//
//		// 2. 다시 재생 (0.f으로 하면 바로 전환)
//		//AnimInstance->Montage_Play(AkaLateMontage, 1.f); // 속도 1배
//
//		// 3. 원하는 섹션으로 점프 (AkaNotify3 근처 섹션 이름 필요)
//		//AnimInstance->Montage_JumpToSection(FName("Fire"), AkaLateMontage);
//		AnimInstance->Montage_Resume(AkaLateMontage);
//		if (AkaProjectile)
//		{
//			// 이동 시작
//			//AkaProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//			AkaProjectile->SetBehaviorType(EProjectileBehaviorType::Move);
//
//			if (GEngine)
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Aka Released!"));
//			}
//
//			// 포인터 초기화 (1회용 발사체라면)
//			AkaProjectile = nullptr;
//		}
//		return;
//	}
//
//
//}
//
//
//void UAka::OnMontageNotify1Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
//{
//	if (NotifyName == FName("AkaNotify1"))
//	{
//
//		UE_LOG(LogTemp, Log, TEXT("Caught Montage Notify: AkaNotify1"));
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Good!"));
//		}
//		// 예: 차징 시작
//		if (AkaProjectile)
//		{
//			// 이동 시작
//			//AkaProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//			AkaProjectile->SetBehaviorType(EProjectileBehaviorType::Move);
//
//			if (GEngine)
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Aka Released!"));
//			}
//
//			// 포인터 초기화 (1회용 발사체라면)
//			AkaProjectile = nullptr;
//		}
//	}
//}
//
//void UAka::OnMontageNotify2Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
//{
//	if (NotifyName == FName("AkaNotify2"))
//	{
//		state = 2;
//		//AnimInstance->Montage_Stop(0.2f); // 0.2초 블렌드 아웃 시간
//		AnimInstance->Montage_Pause(AkaLateMontage);
//		UE_LOG(LogTemp, Log, TEXT("Caught Montage Notify: AkaNotify1"));
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Good!"));
//		}
//		// 예: 차징 시작
//		if (!ProjectileClass || !Owner) return;
//
//		
//	}
//}
//
//void UAka::OnMontageNotify3Begin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
//{
//	if (NotifyName == FName("AkaNotify3"))
//	{
//		UE_LOG(LogTemp, Log, TEXT("Caught Montage Notify: AkaNotify1"));
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Good!"));
//		}
//		// 예: 차징 시작
//	}
//}
