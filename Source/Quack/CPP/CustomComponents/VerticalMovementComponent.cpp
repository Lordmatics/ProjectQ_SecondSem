// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/VerticalMovementComponent.h"


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


// Called every frame
void UVerticalMovementComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	ActivateMovement(DeltaTime);
	// ...
}

void UVerticalMovementComponent::ActivateMovement(float DeltaTime)
{
	if (bActivate)
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		float Start = OwnerLocation.Z;
		// Lower to -110.0f Z
		Start = FMath::FInterpConstantTo(Start, TargetHeight, DeltaTime, InterpRate);
		FVector EndLocation = FVector(OwnerLocation.X, OwnerLocation.Y, Start);
		GetOwner()->SetActorRelativeLocation(EndLocation);
	}
	else
	{
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