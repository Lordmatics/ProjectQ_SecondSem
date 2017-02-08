// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/FloaterComponent.h"


// Sets default values for this component's properties
UFloaterComponent::UFloaterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFloaterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFloaterComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	Float(DeltaTime);
	// ...
}

void UFloaterComponent::Float(float DeltaTime)
{
	if (!bCanFloat)return;
	FVector NewLocation = GetOwner()->GetActorLocation();
	float DeltaHeight = (FMath::Sin(CurrentTime + DeltaTime) - FMath::Sin(CurrentTime));
	NewLocation.Z += DeltaHeight * Height;  //Scale our height by a factor of 20
	CurrentTime += DeltaTime * Speed;
	GetOwner()->SetActorLocation(NewLocation);
}