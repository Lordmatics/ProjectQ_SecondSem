// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/ChildProjectiles/BossTailProjectile.h"
#include "Headers/Character/QuackCharacter.h"



ABossTailProjectile::ABossTailProjectile()
{

}

void ABossTailProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABossTailProjectile::SomethingHit(AActor* OtherActor)
{
	Super::SomethingHit(OtherActor);
//	UE_LOG(LogTemp, Warning, TEXT("%s, was hit CHILD CLASS - TAIL SHOT"), *(OtherActor->GetName()));
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		MyCharacter->DecreaseHealth(ProjectileDamage);
		Destroy();
	}
	else
	{
		Destroy();
	}
}