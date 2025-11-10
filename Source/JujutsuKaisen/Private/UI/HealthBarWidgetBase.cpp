#include "UI/HealthBarWidgetBase.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Characters/JujutsuKaisenCharacter.h"
#include "Math/UnrealMathUtility.h"

void UHealthBarWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ensureMsgf(CharacterNameText, TEXT("CharacterNameText must be bound in the widget blueprint.")))
	{
		return;
	}

	if (!ensureMsgf(HealthProgressBar, TEXT("HealthProgressBar must be bound in the widget blueprint.")))
	{
		return;
	}

	// Initialise to default state.
	SetCharacterDisplayName(FText::GetEmpty());
	SetHealthValues(0.0f, 1.0f);
}

void UHealthBarWidgetBase::InitializeWithCharacter(AJujutsuKaisenCharacter* InCharacter)
{
	LinkedCharacter = InCharacter;

	if (LinkedCharacter.IsValid())
	{
		SetCharacterDisplayName(FText::FromString(LinkedCharacter->GetName()));
		SetHealthValues(LinkedCharacter->GetCurrentHealth(), LinkedCharacter->GetMaxHealthValue());
	}
}

void UHealthBarWidgetBase::SetCharacterDisplayName(const FText& InDisplayName)
{
	if (CharacterNameText)
	{
		CharacterNameText->SetText(InDisplayName);
	}
}

void UHealthBarWidgetBase::SetHealthValues(float CurrentHealth, float MaxHealth)
{
	if (!HealthProgressBar)
	{
		return;
	}

	const float SafeMax = FMath::Max(MaxHealth, KINDA_SMALL_NUMBER);
	const float Percent = FMath::Clamp(CurrentHealth / SafeMax, 0.0f, 1.0f);
	HealthProgressBar->SetPercent(Percent);
}

