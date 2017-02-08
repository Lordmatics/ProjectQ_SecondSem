// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Misc/QuackProjectile.h"
#include "PlasmaGunProjectile.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API APlasmaGunProjectile : public AQuackProjectile
{
	GENERATED_BODY()
	
	
private:

public:
	APlasmaGunProjectile();

	virtual void BeginPlay() override;

	virtual void SomethingHit(AActor* OtherActor) override;
	
};
