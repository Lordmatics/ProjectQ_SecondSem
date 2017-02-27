// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "VerticalMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UVerticalMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	// Lower for Laser Position
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float GroundLaserHeight = 3200.0f;

	// Highest Position // And Return to normal Laser Position
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float StartingHeight = 3775.0f;

	// Lowest position
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float TargetHeight = -750.0f;

	// Constant Rate
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float InterpRate = 750.0f;

	// Bool to control movement
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		uint32 bActivate : 1;

	// Height to match pipe heal animation - upper
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float UpperPipeHealHeight = 4200.0f;

	// Height to match pipe heal animation - lower
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float LowerPipeHealHeight = 3200.0f;

	struct FHeightStructure
	{
		uint32 bOverriden : 1;
		uint32 bIsLower : 1;
		uint32 bLaserOverride : 1;
		FHeightStructure()
		{
			bOverriden = false;
			bIsLower = false;
			bLaserOverride = false;
		}
		FHeightStructure(bool Override, bool IsLower)
		{
			bOverriden = Override;
			bIsLower = IsLower;
			bLaserOverride = false;
		}
		FHeightStructure(bool Override, bool IsLower, bool LaserOverride)
		{
			bOverriden = Override;
			bIsLower = IsLower;
			bLaserOverride = LaserOverride;
		}

	};

	FHeightStructure VerticalHeight = FHeightStructure();

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

	/** Make Owner Change Z Pos to match animation*/
	void AdjustToLowerPipeHeight();

	/** Make Owner Change Z Pos to match animation*/
	void AdjustToUpperPipeHeight(bool LaserOverride = false);

	/** Function to revert back to former height logic*/
	void FinishAdjust();
	
	FORCEINLINE float GetGroundLaserHeight() const { return GroundLaserHeight; }
	FORCEINLINE float GetStartingLaserHeight() const { return StartingHeight; }

};
