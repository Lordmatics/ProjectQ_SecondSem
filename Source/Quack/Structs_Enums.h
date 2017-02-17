// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
UENUM(BlueprintType)        //"BlueprintType" is essential specifier
							//256 entries max
	enum class BossStates : uint8
{
	E_Idle     UMETA(DisplayName = "Idle"),
	E_HealingOne         UMETA(DisplayName = "HealingOne"),
	E_HealingTwo         UMETA(DisplayName = "HealingTwo"),
	E_HealingThree         UMETA(DisplayName = "HealingThree"),
	E_HealingFour         UMETA(DisplayName = "HealingFour"),
	E_Poisoned        UMETA(DisplayName = "Poisoned"),
	E_Recoiling			UMETA(DisplayName = "Recoiling"),
	E_Fighting		UMETA(DisplayName = "Fighting"),
	E_FightingTwo		UMETA(DisplayName = "FightingTwo"),
	E_FightingThree		UMETA(DisplayName = "FightingThree"),
	E_FightingFour		UMETA(DisplayName = "FightingFour"),
};

UENUM(BlueprintType)        //"BlueprintType" is essential specifier
							//256 entries max
	enum class AnimationStates : uint8
{
	E_AnimIdle     UMETA(DisplayName = "AnimIdle"),
	E_AnimLatch         UMETA(DisplayName = "AnimLatch"),
	E_AnimGulp         UMETA(DisplayName = "AnimGulp"),
	E_AnimRecoil         UMETA(DisplayName = "AnimRecoil"),
	E_AnimMelee         UMETA(DisplayName = "AnimMelee"),
	E_AnimTailShot        UMETA(DisplayName = "AnimTailShot"),
	E_AnimBileSpit		UMETA(DisplayName = "AnimBileSpit"),
	E_AnimLatchLower        UMETA(DisplayName = "AnimLatchLower"),
	E_AnimGulpLower		UMETA(DisplayName = "AnimGulpLower"),
	E_AnimRecoilLower		UMETA(DisplayName = "AnimRecoilLower"),
};