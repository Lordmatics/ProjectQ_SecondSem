// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LaserAdjustmentZone.generated.h"

UCLASS()
class QUACK_API ALaserAdjustmentZone : public AActor
{
	GENERATED_BODY()
private:

	// Actually Scrap this idea, just gonna do it based of player height and interp
	// Might adjust VerticalMovementComp for this
	UPROPERTY(VisibleDefaultsOnly, Category = "Box")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Box")
		class UBoxComponent* MyTrigger;

public:	
	// Sets default values for this actor's properties
	ALaserAdjustmentZone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
