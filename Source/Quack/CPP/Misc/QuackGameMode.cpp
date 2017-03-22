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

void AQuackGameMode::BeginPlay()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	TextToSave += LINE_TERMINATOR;

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		// Get absolute file path
		AbsoluteFilePath = SaveDirectory + "/" + FileName + FileType;

		// Allow overwriting or file doesn't already exist
		if (!PlatformFile.FileExists(*AbsoluteFilePath))
		{
			return;
		}
		else
		{
			int Counter = 0;
			while (PlatformFile.FileExists(*AbsoluteFilePath))
			{
				AbsoluteFilePath = SaveDirectory + "/" + FileName + FString::FromInt(Counter) + FileType;
				Counter++;
				if (!PlatformFile.FileExists(*AbsoluteFilePath))
				{
					break;
				}
			}
			// This needs doing at the end, so we get all the data
			//UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SlotName, SaveGameInstance->UserIndex);
		}
	}

}

void AQuackGameMode::AddToString(FString ToAdd)
{
	TextToSave += ToAdd;
}

void AQuackGameMode::WriteToFile()
{
	FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath);
}