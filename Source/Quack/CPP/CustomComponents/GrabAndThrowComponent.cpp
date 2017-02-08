// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/GrabAndThrowComponent.h"
#include "Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include "Headers/Misc/Rock.h"
// Sets default values for this component's properties
UGrabAndThrowComponent::UGrabAndThrowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGrabAndThrowComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UGrabAndThrowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// GRAB AND THROW LOGIC

void UGrabAndThrowComponent::ReleaseRock()
{
	if (CurrentRock != nullptr)
	{
		CurrentRock->bIsHeld = false;
		CurrentRock = nullptr;
	}
	bObjectHeld = false;
}

void UGrabAndThrowComponent::TraceHandleLocation(UPhysicsHandleComponent* PH, UCameraComponent* FPC)
{
	check(PH);
	check(FPC);
	if (bPhysicsHandleActive)
	{
		if (PH != nullptr && FPC != nullptr)
		{
			FVector StartTrace = FPC->GetComponentLocation();
			FVector ForwardVector = FPC->GetForwardVector();
			FVector EndTrace = StartTrace + (ForwardVector * VectorLength);
			HandleLocation = EndTrace;
			PH->SetTargetLocationAndRotation(HandleLocation, GetOwner()->GetActorRotation());
		}
	}
}
void UGrabAndThrowComponent::Throw(UPhysicsHandleComponent* PH, UCameraComponent* FPC, bool MD)
{
	check(PH);
	check(FPC);
	if (PH != nullptr && FPC != nullptr)
	{
		if (bPhysicsHandleActive)
		{
			bPhysicsHandleActive = false;
			PH->ReleaseComponent();
			if (HitComponent != nullptr)
			{
				if (MD)
				{
					ReleaseRock();
					const FVector ForwardVector = FPC->GetForwardVector();
					FVector ImpulseVector = ForwardVector * ThrowForce;
					HitComponent->AddImpulse(ImpulseVector, NAME_None, true);
				}
			}
		}
	}
}

void UGrabAndThrowComponent::Grab(UObject* WCO, UPhysicsHandleComponent* PH, UCameraComponent* FPC)
{
	check(WCO);
	check(FPC);
	if (FPC == nullptr || WCO == nullptr) return;
	FHitResult Hit;
	FVector StartTrace = FPC->GetComponentLocation();
	FVector ForwardVector = FPC->GetForwardVector();
	FVector EndTrace = StartTrace + (ForwardVector * RayLength);
	FVector MyLocation = GetOwner()->GetActorLocation();
	FRotator MyRotation = GetOwner()->GetActorRotation();
	FTransform MyTransform = GetOwner()->GetActorTransform();
	if (UKismetSystemLibrary::LineTraceSingleForObjects(WCO, StartTrace, EndTrace, PhysicsObjType, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true, FColor::Red, FColor::Green))
	{
		if (Hit.GetActor() != nullptr)
		{
			ARock* Rock = Cast<ARock>(Hit.GetActor());
			if (Rock != nullptr)
			{
				Rock->SetGrabOwner(this);
				HitComponent = Hit.GetComponent();
				if (HitComponent != nullptr)
				{
					if (HitComponent->IsSimulatingPhysics() && HitComponent->IsGravityEnabled())
					{
						if (HitComponent->GetMass() < ObjectWeightLimit)
						{
							FVector Distance = MyLocation - HitComponent->GetComponentLocation();
							VectorLength = Distance.Size();
							if (PH != nullptr)
							{
								if (VectorLength <= MinGrabLength)
								{
									if (Rock->RockSize <= MaxRockWeight)
									{
										bPhysicsHandleActive = true;
										CurrentRock = Rock;
										Rock->bIsHeld = true;
										VectorLength = 450.0f;
										bObjectHeld = true;
										PH->GrabComponent(HitComponent, Hit.BoneName, HitComponent->GetCenterOfMass(), true);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

// Release
void UGrabAndThrowComponent::Release(UPhysicsHandleComponent* PH)
{
	check(PH);
	if (PH != nullptr && bObjectHeld)
	{
		bPhysicsHandleActive = false;
		ReleaseRock();
		PH->ReleaseComponent();
	}
}

void UGrabAndThrowComponent::MeleeRockRelease()
{
	bPhysicsHandleActive = false;
	ReleaseRock();
}