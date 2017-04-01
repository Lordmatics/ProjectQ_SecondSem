// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/LaserAdjustmentZone.h"


// Sets default values
ALaserAdjustmentZone::ALaserAdjustmentZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MyTrigger"));
	MyTrigger->bGenerateOverlapEvents = true;
	//MyTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALaserAdjustmentZone::OnTriggerEnter);
	MyTrigger->SetupAttachment(MyRoot);
}

// Called when the game starts or when spawned
void ALaserAdjustmentZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserAdjustmentZone::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//void ALaserAdjustmentZone::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//
//}