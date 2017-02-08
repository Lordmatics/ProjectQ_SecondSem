// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Headers/Quack.h"
#include "Headers/Misc/QuackGameMode.h"
#include "Headers/Character/QuackHUD.h"
#include "Headers/Character/QuackCharacter.h"

AQuackGameMode::AQuackGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintExtensions/BP_QuackCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AQuackHUD::StaticClass();
}
