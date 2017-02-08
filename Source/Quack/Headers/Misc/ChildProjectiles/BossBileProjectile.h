// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Misc/QuackProjectile.h"
#include "BossBileProjectile.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API ABossBileProjectile : public AQuackProjectile
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, Category = "Bile")
		float RandMinVel = 1100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Bile")
		float RandMaxVel = 2000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Bile")
		float RandMinVelScale = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Bile")
		float RandMaxVelScale = 5.0f;

	UPROPERTY(VisibleAnywhere, Category = "Bile")
		float Scalar = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Bile")
		TSubclassOf<class ABileSplashEffect> BileSplashClass;
public:
	ABossBileProjectile();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SomethingHit(AActor* OtherActor) override;

	void SpawnBile(const FVector& HitLocation, const FRotator& HitRotation);
	
	void RandomiseProjection();
	
};
