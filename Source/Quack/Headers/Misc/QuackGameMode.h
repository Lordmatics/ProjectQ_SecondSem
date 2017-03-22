// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "QuackGameMode.generated.h"

UCLASS(minimalapi)
class AQuackGameMode : public AGameMode
{
	GENERATED_BODY()


	FString SaveDirectory = FString("E:/Unreal/TeamProject/Master/Saved/TestingResults");

	FString FileName = FString("TestResults");

	FString FileType = FString(".txt");

	FString TextToSave = FString("Testing Results: ");

	FString AbsoluteFilePath;

	virtual void BeginPlay() override;

public:
	AQuackGameMode();

	void AddToString(FString ToAdd);

	void WriteToFile();
};



