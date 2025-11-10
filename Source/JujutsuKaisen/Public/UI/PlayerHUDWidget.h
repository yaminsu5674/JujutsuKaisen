#pragma once

#include "Blueprint/UserWidget.h"
#include "Math/Vector2D.h"
#include "PlayerHUDWidget.generated.h"

class AJujutsuKaisenCharacter;

class UPlayerHealthBarWidget;
class UEnemyHealthBarWidget;
class UJujutsuKaisenCharacterDataAsset;

/**
 * HUD widget managing player and enemy health bars.
 * Designed to be extended in Blueprint for layout and positioning.
 */
UCLASS(Blueprintable)
class JUJUTSUKAISEN_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Sets the player health bar instance displayed on the HUD. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetPlayerHealthBar(UPlayerHealthBarWidget* InPlayerWidget);

	/** Sets the enemy health bar instance displayed on the HUD. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetEnemyHealthBar(UEnemyHealthBarWidget* InEnemyWidget);

	/** Provides access to the player health bar widget. */
	UFUNCTION(BlueprintPure, Category = "HUD")
	UPlayerHealthBarWidget* GetPlayerHealthBar() const { return PlayerHealthBar; }

	/** Provides access to the enemy health bar widget. */
	UFUNCTION(BlueprintPure, Category = "HUD")
	UEnemyHealthBarWidget* GetEnemyHealthBar() const { return EnemyHealthBar; }

	/**
	 * Updates the enemy health bar position so it hovers over the specified world location.
	 * Typically called every tick after projecting the enemy's world position via Blueprint.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateEnemyHealthBarScreenPosition(const FVector2D& ScreenPosition);

	/**
	 * Calculates and updates the enemy health bar screen-space position based on the enemy's head location.
	 * This should be called periodically (e.g., each tick) to keep the bar aligned.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateEnemyHealthBarForCharacter(AJujutsuKaisenCharacter* EnemyCharacter);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetEnemyDisplayName(const FText& DisplayName);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetPlayerDisplayName(const FText& DisplayName);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget, AllowPrivateAccess = "true"))
	UPlayerHealthBarWidget* PlayerHealthBar = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget, AllowPrivateAccess = "true"))
	UEnemyHealthBarWidget* EnemyHealthBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	float EnemyBarHeightOffset = 150.0f;
};

