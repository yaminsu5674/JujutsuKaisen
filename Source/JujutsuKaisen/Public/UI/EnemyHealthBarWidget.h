#pragma once

#include "UI/HealthBarWidgetBase.h"
#include "EnemyHealthBarWidget.generated.h"

/**
 * Enemy-specific health bar widget.
 * Extend in Blueprint to customise visuals or behaviour.
 */
UCLASS(Blueprintable)
class JUJUTSUKAISEN_API UEnemyHealthBarWidget : public UHealthBarWidgetBase
{
	GENERATED_BODY()
};

