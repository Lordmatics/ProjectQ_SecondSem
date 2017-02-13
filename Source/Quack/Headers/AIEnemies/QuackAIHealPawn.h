// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/AIEnemies/QuackAIPawn.h"
#include "QuackAIHealPawn.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AQuackAIHealPawn : public AQuackAIPawn
{
	GENERATED_BODY()

		AQuackAIHealPawn();

	virtual void BeginPlay() override;

		virtual void Attack() override;

		virtual void Tick(float DeltaTime) override;

		void Heal();


	class APipe* Pipe;
	
};
