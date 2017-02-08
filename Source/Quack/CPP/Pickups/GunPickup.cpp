// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Pickups/GunPickup.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/CustomComponents/InteractComponent.h"

// Sets default values
AGunPickup::AGunPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	MySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MySkeletalMesh"));
	MySkeletalMesh->SetupAttachment(MyRoot);

	MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MyTrigger"));
	MyTrigger->bGenerateOverlapEvents = true;
	MyTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGunPickup::OnTriggerEnter);
	MyTrigger->OnComponentEndOverlap.AddDynamic(this, &AGunPickup::OnTriggerExit);
	MyTrigger->SetupAttachment(MySkeletalMesh);

	InteractClass = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractClass"));
}

// Called when the game starts or when spawned
void AGunPickup::BeginPlay()
{
	Super::BeginPlay();
	if (InteractClass != nullptr)
	{
		// 0 gun, 1 needle
		InteractClass->SetID(0);
	}
	
}

// Called every frame
void AGunPickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGunPickup::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		if (InteractClass != nullptr)
			InteractClass->SetPickUpable(true);
		else
			UE_LOG(LogTemp, Warning, TEXT("InteractClass NULLPTR"));
	}
}

void AGunPickup::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		if (InteractClass != nullptr)
			InteractClass->SetPickUpable(false);
		else
			UE_LOG(LogTemp, Warning, TEXT("InteractClass NULLPTR"));
	}
}
