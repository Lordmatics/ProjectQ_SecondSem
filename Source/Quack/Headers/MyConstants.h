// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyConstants.generated.h"

UENUM(BlueprintType)        //"BlueprintType" is essential specifier
enum class AudioStates : uint8
{
	E_BossIntro = 0     UMETA(DisplayName = "BossIntro"),
	E_BossLoop = 1         UMETA(DisplayName = "BossLoop"),
	E_CombatLoop = 2         UMETA(DisplayName = "Combat"),
	E_EnemiesNearLoop = 3         UMETA(DisplayName = "EnemiesNear"),
	E_ExplorationLoop = 4         UMETA(DisplayName = "Exploring"),
	E_Victory = 5        UMETA(DisplayName = "Victory"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyConstants : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class QUACK_API IMyConstants
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	
};
