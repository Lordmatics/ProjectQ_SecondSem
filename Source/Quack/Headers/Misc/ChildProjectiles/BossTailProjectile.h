// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Misc/QuackProjectile.h"
#include "BossTailProjectile.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API ABossTailProjectile : public AQuackProjectile
{
	GENERATED_BODY()
	
	
private:

public:
	ABossTailProjectile();

	virtual void BeginPlay() override;
	
	virtual void SomethingHit(AActor* OtherActor) override;
};
