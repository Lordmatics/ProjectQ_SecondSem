// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

USTRUCT(BlueprintType)
struct FElevatorPositions
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float ElevatorTarget;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float ElevatorStart;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float ElevatorMinionTargetOne;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float ElevatorMinionTargetTwo;

	/** defaults */
	FElevatorPositions()
	{
		//bIsUsingGravityGun = false;
		//bHasGravityGun = false;
		ElevatorTarget = 152.147598f;
		ElevatorStart = -10000.0f;
		ElevatorMinionTargetOne = -5000.0f;
		ElevatorMinionTargetTwo = -1000.0f;
	}
};

UCLASS()
class QUACK_API AElevator : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		UStaticMeshComponent* ElevatorMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		UStaticMeshComponent* SmashablePanel;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		UStaticMeshComponent* LeftElevatorDoor;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		UStaticMeshComponent* RightElevatorDoor;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		UBoxComponent* SpawnZone;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		class UDestructibleComponent* SmashablePanelDM;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		class URadialForceComponent* RadialForce;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		class UVerticalMovementComponent* MovementComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		class UMinionFactoryComponent* MinionFactoryComp;

private:
	UPROPERTY(EditAnywhere, Category = "Elevator")
		float ElevatorSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float CollisionTimerDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float DestroyPanelDelay = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 bElevatorStop : 1;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 EnemiesToSpawn = 3;
	
	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 bDefeatedWave : 1;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 bDefeatedBear : 1;

	/** Elevator Stops Data */
	UPROPERTY(EditAnywhere, Category = "Elevator Positions")
		FElevatorPositions ElevatorPositions = FElevatorPositions();
private:

	UFUNCTION()
		void OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection);

	UFUNCTION()
		void DisableCollision();

	UFUNCTION()
		void DestroyThis();
public:	
	// Sets default values for this actor's properties
	AElevator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	
};
