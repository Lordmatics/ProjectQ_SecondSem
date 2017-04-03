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
		ElevatorTarget = 152.0f;
		ElevatorStart = -10000.0f;
		ElevatorMinionTargetOne = -7500.0f;
		ElevatorMinionTargetTwo = -3000.0f;
	}
};

USTRUCT(BlueprintType)
struct FDoorPositions
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float LeftDoorTargetX;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float RightDoorTargetX;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float LeftDoorStartX;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float RightDoorStartX;

	/** defaults */
	FDoorPositions()
	{
		//bIsUsingGravityGun = false;
		//bHasGravityGun = false;
		LeftDoorTargetX = -725.0f;
		RightDoorTargetX = -75.0f;
		LeftDoorStartX = -365.0f;
		RightDoorStartX = -495.0f;
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

	UPROPERTY(EditDefaultsOnly, Category = "C++ Cutscenes")
		class UMatineeContainerComponent* CutsceneContainer;

	UPROPERTY(VisibleDefaultsOnly, Category = "Elevator")
		class UBossArmourComponent* ArmourComp;

	UPROPERTY(VisibleAnywhere, Category = "C++ Armour References")
		class AQuackArmourPin* PinRefUL;

	UPROPERTY(VisibleAnywhere, Category = "C++ Armour References")
		class AQuackArmourPin* PinRefUR;

private:

	FTimerHandle ResumeMovementHandle;
	UFUNCTION()
		void ResumeMovement();

	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<UCameraShake> ElevatorShake;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float ElevatorSpeed = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float DoorSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float CollisionTimerDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		float DestroyPanelDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 bElevatorStop : 1;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 EnemiesToSpawn = 3;
	
	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 bDefeatedWave : 1;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 bDefeatedBear : 1;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 bOpenDoors : 1;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		uint32 bPlayPinCutsceneOnce : 1;

	/** Elevator Stops Data */
	UPROPERTY(EditAnywhere, Category = "Elevator Positions")
		FElevatorPositions ElevatorPositions = FElevatorPositions();

	UPROPERTY(EditAnywhere, Category = "Elevator Door Positions")
		FDoorPositions DoorPositions = FDoorPositions();
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

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void CloseDoors();
	
};
