// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Pickups/HealthPickUp.h"
#include "Headers/Character/QuackCharacter.h"

// Sets default values
AHealthPickUp::AHealthPickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHealthPickUp::BeginPlay()
{
	Super::BeginPlay();

	if (MyStaticMesh != nullptr)
	{
		MyStaticMesh->bGenerateOverlapEvents = true;
		MyStaticMesh->OnComponentBeginOverlap.RemoveDynamic(this, &APickUpBase::OnTriggerEnter);
		MyStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickUp::OnTriggerEnter);
	}
}

// Called every frame
void AHealthPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPickUp::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		// Add to Inventory or something
		MyCharacter->IncreaseHealth(HealthCrateValue);
		// Destroy Pick Up
		Destroy();
	}
}


