// Copyright Epic Games, Inc. All Rights Reserved.

#include "JujutsuKaisenGameMode.h"
#include "JujutsuKaisenCharacter.h"
#include "UObject/ConstructorHelpers.h"

AJujutsuKaisenGameMode::AJujutsuKaisenGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
