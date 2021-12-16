// Copyright Epic Games, Inc. All Rights Reserved.

#include "LostArkGameMode.h"
#include "LostArkCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALostArkGameMode::ALostArkGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
