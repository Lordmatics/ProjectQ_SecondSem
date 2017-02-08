// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Stations/HealthStation.h"
#include "Classes/Components/ShapeComponent.h"
#include "Headers/Character/QuackCharacter.h"

// Sets default values
AHealthStation::AHealthStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRootComponent;

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMesh"));
	MyStaticMesh->SetupAttachment(MyRootComponent);

	MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MyTrigger"));
	MyTrigger->SetupAttachment(MyStaticMesh);
}

// Called when the game starts or when spawned
void AHealthStation::BeginPlay()
{
	Super::BeginPlay();
	PlaySound();
	InitialiseCharacterReference();
}

void AHealthStation::InitialiseCharacterReference()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AQuackCharacter> ActorItr(World); ActorItr; ++ActorItr)
		{
			MyCharacter = *ActorItr;
		}
	}
}

void AHealthStation::PlaySound()
{
	if (JuggernogSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, JuggernogSound, GetActorLocation(), Volume);
	}
}
// Called every frame
void AHealthStation::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	switch (bIsInZone)
	{
		case true:
		{
			// Check if station has capacity
			if (MyCharacter != nullptr)
			{
				if (HealingCapacity > 0.5f)
				{
					if (MyCharacter->GetCurrentHealth() < MyCharacter->GetMaxHealth() - 0.25f)
					{
						ToggleHighlight(true);
						//PlaySound();
						// If capacity is below rate , consume the rest
						if (HealingCapacity <= HealingAmount)
						{
							float RemainingHeal = HealingCapacity;
							MyCharacter->IncreaseHealth(RemainingHeal, DeltaTime);
							HealingCapacity -= DeltaTime * RemainingHeal;
							HealingCapacity = FMath::Clamp(HealingCapacity, 0.0f, 100.0f);
						}
						else
						{
							MyCharacter->IncreaseHealth(HealingAmount, DeltaTime);
							HealingCapacity -= DeltaTime * HealingAmount;
							HealingCapacity = FMath::Clamp(HealingCapacity, 0.0f, 100.0f);
						}
					}
				}
				else
				{
					ToggleHighlight(false);
				}
			}
			break;
		}
		case false:
		{
			// Not in zone
			ToggleHighlight(false);
			break;
		}
	}

}

void AHealthStation::ToggleHighlight(bool Glow)
{
	//MyStaticMesh->SetCustomDepthStencilValue((uint8)Colour);
	MyStaticMesh->SetRenderCustomDepth(Glow);

}

