#pragma once

#include "Blueprint/UserWidget.h"
#include "HealthBarWidgetBase.generated.h"

class UProgressBar;
class UTextBlock;
class AJujutsuKaisenCharacter;

/**
 * Base widget for character health bars.
 * Provides name display and health progress functionality.
 */
UCLASS(Abstract, Blueprintable)
class JUJUTSUKAISEN_API UHealthBarWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Link this widget to a specific character instance. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void InitializeWithCharacter(AJujutsuKaisenCharacter* InCharacter);

	/** Update the textual display of the character's name. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCharacterDisplayName(const FText& InDisplayName);

	/** Apply current/max health values to the progress bar. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealthValues(float CurrentHealth, float MaxHealth);

	/** Retrieve the character associated with this widget. */
	UFUNCTION(BlueprintPure, Category = "Health")
	AJujutsuKaisenCharacter* GetLinkedCharacter() const { return LinkedCharacter.Get(); }

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AJujutsuKaisenCharacter> LinkedCharacter;

	/** Character display name text block – must be bound in the widget blueprint. */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* CharacterNameText;

	/** Health progress bar – must be bound in the widget blueprint. */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UProgressBar* HealthProgressBar;
};

