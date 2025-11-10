#include "UI/PlayerHUDWidget.h"

#include "UI/PlayerHealthBarWidget.h"
#include "UI/EnemyHealthBarWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerHUDWidget::SetPlayerHealthBar(UPlayerHealthBarWidget* InPlayerWidget)
{
	PlayerHealthBar = InPlayerWidget;
}

void UPlayerHUDWidget::SetEnemyHealthBar(UEnemyHealthBarWidget* InEnemyWidget)
{
	EnemyHealthBar = InEnemyWidget;
}

void UPlayerHUDWidget::UpdateEnemyHealthBarScreenPosition(const FVector2D& ScreenPosition)
{
	if (!EnemyHealthBar)
	{
		return;
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(EnemyHealthBar->Slot))
	{
		//CanvasSlot->SetAlignment(FVector2D(0.5f, 1.0f));
		CanvasSlot->SetPosition(ScreenPosition);
	}
}

void UPlayerHUDWidget::UpdateEnemyHealthBarForCharacter(AJujutsuKaisenCharacter* EnemyCharacter)
{
	if (!EnemyHealthBar || !EnemyCharacter)
	{
		return;
	}

	APlayerController* OwningPC = GetOwningPlayer();
	if (!OwningPC)
	{
		return;
	}

	const FVector HeadWorldLocation = EnemyCharacter->GetActorLocation() + FVector(0.0f, 0.0f, EnemyBarHeightOffset);

	FVector2D ScreenPosition;
	const bool bProjected = UGameplayStatics::ProjectWorldToScreen(OwningPC, HeadWorldLocation, ScreenPosition);

	if (!bProjected)
	{
		return;
	}

	UpdateEnemyHealthBarScreenPosition(ScreenPosition);
}

void UPlayerHUDWidget::SetEnemyDisplayName(const FText& DisplayName)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetCharacterDisplayName(DisplayName);
	}
}

void UPlayerHUDWidget::SetPlayerDisplayName(const FText& DisplayName)
{
	if (PlayerHealthBar)
	{
		PlayerHealthBar->SetCharacterDisplayName(DisplayName);
	}
}

