// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "MinionSpawnPoint.h"


// Sets default values
AMinionSpawnPoint::AMinionSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMinionSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinionSpawnPoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

