// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "VerticalMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UVerticalMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// Highest Position
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float StartingHeight = 1630.0f;

	// Lowest position
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float TargetHeight = -110.0f;

	// Constant Rate
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float InterpRate = 100.0f;

	// Bool to control movement
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		uint32 bActivate : 1;

private:

	void ActivateMovement(float DeltaTime);

public:	
	// Sets default values for this component's properties
	UVerticalMovementComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/** Make Owner descend into Target Height*/
	void Lower();

	/** Make Owner Ascend into Starting Height*/
	void Raise();
	
};
