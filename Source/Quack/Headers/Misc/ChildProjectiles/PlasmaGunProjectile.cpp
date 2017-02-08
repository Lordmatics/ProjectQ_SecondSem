// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/ChildProjectiles/PlasmaGunProjectile.h"

APlasmaGunProjectile::APlasmaGunProjectile()
{

}

void APlasmaGunProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void APlasmaGunProjectile::SomethingHit(AActor* OtherActor)
{
	Super::SomethingHit(OtherActor);

}