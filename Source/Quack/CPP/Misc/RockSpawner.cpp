// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/RockSpawner.h"
#include "Headers/Misc/Rock.h"

// Sets default values
ARockSpawner::ARockSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARockSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnRocks();
}

// Called every frame
void ARockSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ARockSpawner::SpawnRocks()
{
	if (RockClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL && SpawnPoints.Num() != 0)
		{
			for (size_t i = 0; i < SpawnPoints.Num(); i++)
			{
				const FRotator SpawnRotation = SpawnPoints[i]->GetActorRotation();
				const FVector SpawnLocation = SpawnPoints[i]->GetActorLocation();
				ARock* Rock = World->SpawnActor<ARock>(RockClass, SpawnLocation, SpawnRotation);
				if (Rock != nullptr)
				{
					// 50% chance to be heavy or not
					int Random = FMath::RandRange(0, 1);
					switch (Random)
					{
						case 0:
						{
							Rock->RockSize = 50.0f;
							break;
						}
						case 1:
						{
							Rock->RockSize = 150.0f;
							Rock->SetActorScale3D(FVector(1.5f));
							Rock->RockDamage = 0.0f;
							Rock->SetHeavyMesh();
							break;
						}
					}
					ActiveRocks.Add(Rock);

				}
			}		
		}
	}
}

void ARockSpawner::ReplaceFallingRocks(int SpawnPoint)
{
	if (RockClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL && SpawnPoints.Num() != 0)
		{
			const FRotator SpawnRotation = SpawnPoints[SpawnPoint]->GetActorRotation();
			const FVector SpawnLocation = SpawnPoints[SpawnPoint]->GetActorLocation();
			ARock* Rock = World->SpawnActor<ARock>(RockClass, SpawnLocation, SpawnRotation);
			if(Rock != nullptr)
				ActiveRocks.Add(Rock);		
		}
	}
}

