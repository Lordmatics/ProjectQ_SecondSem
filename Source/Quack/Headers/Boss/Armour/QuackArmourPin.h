// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Boss/Armour/QuackBossArmourBaseClass.h"
#include "QuackArmourPin.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AQuackArmourPin : public AQuackBossArmourBaseClass
{
	GENERATED_BODY()
	
private:
	AQuackArmourPin();
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
protected:
	
	
};