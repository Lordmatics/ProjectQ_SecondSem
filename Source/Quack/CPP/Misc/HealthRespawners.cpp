// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/HealthRespawners.h"
#include "Headers/Pickups/HealthPickUp.h"

// Sets default values
AHealthRespawners::AHealthRespawners()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;
}

// Called when the game starts or when spawned
void AHealthRespawners::BeginPlay()
{
	Super::BeginPlay();
	// Bind Respawn Event, gets activated in respective health pickup
	OnHealthPickedup.AddDynamic(this, &AHealthRespawners::RespawnHealthPack);
}

// Called every frame
void AHealthRespawners::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AHealthRespawners::RespawnHealthPack(AHealthPickUp* _HealthPackToRespawn)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (_HealthPackToRespawn != nullptr)
	{
		SpawnLoc = _HealthPackToRespawn->GetActorLocation();
		SpawnRot = _HealthPackToRespawn->GetActorRotation();
		FTimerHandle TempHandle;
		World->GetTimerManager().SetTimer(TempHandle, this, &AHealthRespawners::DoRespawn, HealthRespawnDelay, false);
	}

}

void AHealthRespawners::DoRespawn()
{
	UWorld* const World = GetWorld();
	if (World == nullptr || HealthClass == nullptr) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AHealthPickUp* HealthPickup = World->SpawnActor<AHealthPickUp>(HealthClass, SpawnLoc, SpawnRot, SpawnParams);
	if (HealthPickup != nullptr)
	{
		HealthPickup->SetRespawnerRef(this);
	}
}