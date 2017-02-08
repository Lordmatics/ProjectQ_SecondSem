// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Pickups/AmmoPickUp.h"
#include "Headers/Character/QuackCharacter.h"

// Sets default values
AAmmoPickUp::AAmmoPickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();

	if (MyStaticMesh != nullptr)
	{
		MyStaticMesh->bGenerateOverlapEvents = true;
		MyStaticMesh->OnComponentBeginOverlap.RemoveDynamic(this, &APickUpBase::OnTriggerEnter);
		MyStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickUp::OnTriggerEnter);
	}
}

// Called every frame
void AAmmoPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoPickUp::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		// Add to Inventory or something
		MyCharacter->IncreaseAmmo(AmmoCrateValue);
		// Destroy Pick Up
		Destroy();
	}
}

