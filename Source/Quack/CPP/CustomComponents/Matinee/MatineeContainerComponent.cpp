// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/Matinee/MatineeContainerComponent.h"
#include "Classes/Matinee/MatineeActor.h"
#include "Classes/Matinee/InterpData.h"

// Sets default values for this component's properties
UMatineeContainerComponent::UMatineeContainerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMatineeContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMatineeContainerComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

float UMatineeContainerComponent::GetMatineeLength(const AMatineeActor* MatineeInQuestion)
{
	if (MatineeInQuestion != nullptr)
	{	
		if (MatineeInQuestion->MatineeData != nullptr)
		{
			return (MatineeInQuestion->MatineeData->InterpLength * MatineeInQuestion->PlayRate);
		}
		UE_LOG(LogTemp, Warning, TEXT("ERROR: Matinee Data was nullptr"));
	}
	UE_LOG(LogTemp, Warning, TEXT("ERROR: Either Matinee was NULL OR MATINEE LENGTH <= 0.0f"));
	return 0.0f;
}

bool UMatineeContainerComponent::PlayLowerPipeRightMatinee()
{
	// Returns Success - if logic is dependant on this - like immobility
	if (Matinees.Num() == 0) return false;
	if (Matinees[LowerPipeRightMatinee] != nullptr)
	{
		Matinees[LowerPipeRightMatinee]->Play();
		UE_LOG(LogTemp, Warning, TEXT("Matinee Played"));
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("ERROR: LowerPipeMatinee Failed"));
	return false;
}

bool UMatineeContainerComponent::PlayLowerPipeLeftMatinee()
{
	// Returns Success - if logic is dependant on this - like immobility
	if (Matinees.Num() == 0) return false;
	if (Matinees[LowerPipeLeftMatinee] != nullptr)
	{
		Matinees[LowerPipeLeftMatinee]->Play();
		UE_LOG(LogTemp, Warning, TEXT("Matinee Played"));
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("ERROR: LowerPipeMatinee Failed"));
	return false;
}

bool UMatineeContainerComponent::PlayUpperPipeMatinee()
{
	// Returns Success - if logic is dependant on this - like immobility
	if (Matinees.Num() == 0) return false;
	if (Matinees[UpperPipeMatinee] != nullptr)
	{
		Matinees[UpperPipeMatinee]->Play();
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("ERROR: UpperPipeMatinee Failed"));
	return false;
}

bool UMatineeContainerComponent::PlayMatineeAt(int Index)
{
	if (Matinees.Num() == 0 || Index >= Matinees.Num()) return false;
	if (Matinees[Index] == nullptr) return false;
	Matinees[Index]->Play();
	return true;
}