#pragma once

#include "UI/HealthBarWidgetBase.h"
#include "PlayerHealthBarWidget.generated.h"

/**
 * Player-specific health bar widget.
 * Can be extended in Blueprint for custom visuals.
 */
UCLASS(Blueprintable)
class JUJUTSUKAISEN_API UPlayerHealthBarWidget : public UHealthBarWidgetBase
{
	GENERATED_BODY()
};

