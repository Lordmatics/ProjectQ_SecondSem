// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class QUACK_API AElevator : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Elevator")
		UStaticMeshComponent* ElevatorMesh;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		FVector StartPos;

	UPROPERTY(EditAnywhere, Category = "Elevator")
		FVector EndPos;
	
	UPROPERTY(EditAnywhere, Category = "Elevator")
		float ElevatorSpeed;
public:	
	// Sets default values for this actor's properties
	AElevator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	
};
