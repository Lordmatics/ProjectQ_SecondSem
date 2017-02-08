// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Pickups/NeedlePickup.h"
#include "Headers/CustomComponents/InteractComponent.h"

// Sets default values
ANeedlePickup::ANeedlePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MyTrigger"));
	MyTrigger->bGenerateOverlapEvents = true;
	//MyTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGunPickup::OnTriggerEnter);
	//MyTrigger->OnComponentEndOverlap.AddDynamic(this, &AGunPickup::OnTriggerExit);
	MyTrigger->SetupAttachment(MyRoot);

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMesh"));
	MyStaticMesh->SetupAttachment(MyTrigger);

	InteractClass = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractClass"));
}

// Called when the game starts or when spawned
void ANeedlePickup::BeginPlay()
{
	Super::BeginPlay();
	if (InteractClass != nullptr)
	{
		// 0 gun, 1 needle
		InteractClass->SetID(1);
	}
}

// Called every frame
void ANeedlePickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

