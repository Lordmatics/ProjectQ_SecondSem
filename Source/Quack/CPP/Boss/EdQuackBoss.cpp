// Sets default values
#include "Headers/Quack.h"
#include "Headers/Boss/EdQuackBoss.h"

AEdQuackBoss::AEdQuackBoss()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEdQuackBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AEdQuackBoss::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEdQuackBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Fill out your copyright notice in the Description page of Project Settings.
