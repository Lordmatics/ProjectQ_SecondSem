// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Pickups/PickUpBase.h"
#include "Headers/Character/QuackCharacter.h"

// Sets default values
APickUpBase::APickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//MyRoot->SetWorldLocation(FVector(0.0f));
	RootComponent = MyRoot;

	//MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickUpTrigger"));
	//MyTrigger->RegisterComponent();
	//MyTrigger->SetupAttachment(MyRoot);
	//MyTrigger->SetRelativeLocation(FVector::ZeroVector);

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	//MyStaticMesh->RegisterComponent();
	MyStaticMesh->SetupAttachment(MyRoot);
	//MyStaticMesh->SetRelativeLocation(FVector::ZeroVector);
	//MyStaticMesh->SetWorldLocation(FVector(0.0f));


	//MyTrigger->SetWorldLocation(FVector(0.0f));

}

// Called when the game starts or when spawned
void APickUpBase::BeginPlay()
{
	Super::BeginPlay();

	if (MyStaticMesh != nullptr)
	{
		MyStaticMesh->bGenerateOverlapEvents = true;
		MyStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &APickUpBase::OnTriggerEnter);
	}
}

// Called every frame
void APickUpBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickUpBase::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		// Add to Inventory or something
		
		// Destroy Pick Up
		Destroy();
	}
}