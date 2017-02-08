// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RockSpawner.generated.h"

UCLASS()
class QUACK_API ARockSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
		TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
	TSubclassOf<class ARock> RockClass;

	void SpawnRocks();
	void ReplaceFallingRocks(int SpawnPoint);

	UPROPERTY(VisibleAnywhere, Category = "Custom Niall")
		TArray<class ARock*> ActiveRocks;
	
};
