// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HealthRespawners.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthConsumed, class AHealthPickUp*, HealthPickup);

UCLASS()
class QUACK_API AHealthRespawners : public AActor
{
	GENERATED_BODY()
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Health Respawner")
		USceneComponent* MyRoot;

	UPROPERTY(EditAnywhere, Category = "Delay")
		float HealthRespawnDelay = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Delay")
		TSubclassOf<class AHealthPickUp> HealthClass;
	//UPROPERTY(VisibleAnywhere, Category = "C++ HealthPack Ref")
	//	class AHealthPickUp* HealthRef;

	FVector SpawnLoc;
	FRotator SpawnRot;
private:

	UFUNCTION()
		void RespawnHealthPack(class AHealthPickUp* _HealthPackToRespawn);

	UFUNCTION()
		void DoRespawn();

public:	
	// Sets default values for this actor's properties
	AHealthRespawners();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, Category = "C++ Delegate")
		FOnHealthConsumed OnHealthPickedup;
	
};
