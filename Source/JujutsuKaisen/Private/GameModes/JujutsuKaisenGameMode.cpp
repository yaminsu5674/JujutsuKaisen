// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/JujutsuKaisenGameMode.h"
#include "Characters/JujutsuKaisenCharacter.h"
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
