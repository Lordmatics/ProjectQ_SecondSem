// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/AIEnemies/QuackAIPawn.h"
#include "QuackAIChargePawn.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AQuackAIChargePawn : public AQuackAIPawn
{
	GENERATED_BODY()
	
		virtual void Die() override;
	
	UFUNCTION()
		void DestroyThis();

public:

	UFUNCTION(BlueprintCallable, Category = "Anim")
		bool GetDeath();
	
};
