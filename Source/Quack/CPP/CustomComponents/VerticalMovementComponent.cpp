// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/VerticalMovementComponent.h"

//#include "Headers/Misc/Elevator.h"

// Sets default values for this component's properties
UVerticalMovementComponent::UVerticalMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVerticalMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//void UVerticalMovementComponent::BeginRaise(FElevatorPositions ElevatorPos)
//{
//
//}

// Called every frame
void UVerticalMovementComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	ActivateMovement(DeltaTime);

	// ...
}

void UVerticalMovementComponent::ActivateMovement(float DeltaTime)
{
	// okay, so basically, in the boss code, when he goes to heal, you will call the
	// respective adjust height function according to targettedpipe.blowerpipe
	// and here it will raise / lower him just enough for the animation
	// once that is finished, call finish adjust, so the latter logic can resume
	if (VerticalHeight.bOverriden)
	{
		//UE_LOG(LogTemp, Warning, TEXT("VerticalHeightOverriden"));
		// Sink to lower pipe
		if (VerticalHeight.bIsLower)
		{
			//UE_LOG(LogTemp, Warning, TEXT("VerticalHeightOverriden - Change to Lower Pipe"));
			FVector OwnerLocation = GetOwner()->GetActorLocation();
			float Start = OwnerLocation.Z;
			// Lower to -110.0f Z
			Start = FMath::FInterpConstantTo(Start, LowerPipeHealHeight, DeltaTime, InterpRate);
			FVector EndLocation = FVector(OwnerLocation.X, OwnerLocation.Y, Start);
			GetOwner()->SetActorRelativeLocation(EndLocation);
			return;
		}
		// Raise to upper pipe
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("VerticalHeightOverriden - Change to Upper Pipe"));
			FVector OwnerLocation = GetOwner()->GetActorLocation();
			float Start = OwnerLocation.Z;
			// Lower to -110.0f Z
			if (VerticalHeight.bLaserOverride)
			{
				Start = FMath::FInterpConstantTo(Start, StartingHeight, DeltaTime, InterpRate);
			}
			else
			{
				Start = FMath::FInterpConstantTo(Start, UpperPipeHealHeight, DeltaTime, InterpRate);
			}
			FVector EndLocation = FVector(OwnerLocation.X, OwnerLocation.Y, Start);
			GetOwner()->SetActorRelativeLocation(EndLocation);
			return;
		}
	}
	// Lower To fog
	if (bActivate)
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		float Start = OwnerLocation.Z;
		// Lower to -110.0f Z
		Start = FMath::FInterpConstantTo(Start, TargetHeight, DeltaTime, InterpRate);
		FVector EndLocation = FVector(OwnerLocation.X, OwnerLocation.Y, Start);
		GetOwner()->SetActorRelativeLocation(EndLocation);
	}
	// Raise to initial
	else
	{
		if (bStop) return;
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		float Start = OwnerLocation.Z;
		// Raise to 1630.0f Z 
		Start = FMath::FInterpConstantTo(Start, StartingHeight, DeltaTime, InterpRate);
		FVector EndLocation = FVector(OwnerLocation.X, OwnerLocation.Y, Start);
		GetOwner()->SetActorRelativeLocation(EndLocation);
	}
}

void UVerticalMovementComponent::Lower()
{
	bActivate = true;
}

void UVerticalMovementComponent::Raise()
{
	bActivate = false;
}

void UVerticalMovementComponent::AdjustToLowerPipeHeight()
{
	VerticalHeight = FHeightStructure(true, true);
}

void UVerticalMovementComponent::AdjustToUpperPipeHeight(bool ALaserOverride)
{
	if (ALaserOverride)
	{
		VerticalHeight = FHeightStructure(true, false, true);
	}
	else
	{
		VerticalHeight = FHeightStructure(true, false);
	}
}

void UVerticalMovementComponent::FinishAdjust()
{
	VerticalHeight = FHeightStructure();
	//UE_LOG(LogTemp, Warning, TEXT("Finished Adjusting Height"));
}