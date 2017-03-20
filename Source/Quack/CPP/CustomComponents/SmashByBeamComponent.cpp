// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/SmashByBeamComponent.h"
#include "Headers/Misc/DestructibleInnerPipes.h"

// Sets default values for this component's properties
USmashByBeamComponent::USmashByBeamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashByBeamComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USmashByBeamComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void USmashByBeamComponent::OwnerFracture()
{
	ADestructibleInnerPipes* PotentialOwner = Cast<ADestructibleInnerPipes>(GetOwner());
	if (PotentialOwner != nullptr && !IsPendingKill())
	{
		UE_LOG(LogTemp, Warning, TEXT("FractureSucces + Component Self Eliminated"));
		PotentialOwner->Fracture();
		DestroyComponent();
	}
}
