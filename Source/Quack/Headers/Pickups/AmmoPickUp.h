// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Pickups/PickUpBase.h"
#include "AmmoPickUp.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AAmmoPickUp : public APickUpBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AAmmoPickUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		virtual void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
	int	AmmoCrateValue = 30;
	
};
