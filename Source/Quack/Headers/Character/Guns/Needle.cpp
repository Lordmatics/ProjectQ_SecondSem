// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Needle.h"
#include "Headers/Character/QuackCharacter.h"

ANeedle::ANeedle()
{
	TheRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));
	RootComponent = TheRoot;

	NeedleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NeedleMesh"));
	NeedleMesh->bReceivesDecals = false;
	NeedleMesh->CastShadow = false;
	NeedleMesh->SetupAttachment(RootComponent);
}

void ANeedle::BeginPlay()
{
	Super::BeginPlay();


}

void ANeedle::Tick(float DeltaSeconds)
{

}

void ANeedle::Shoot()
{

}

void ANeedle::WieldAndActivate()
{
	Super::WieldAndActivate();
	bIsActive = true;
	if (NeedleMesh != nullptr)
	{
		NeedleMesh->SetHiddenInGame(false);
	}
}

void ANeedle::SheathAndDeactivate()
{
	Super::SheathAndDeactivate();
	bIsActive = false;
	if (NeedleMesh != nullptr)
	{
		NeedleMesh->SetHiddenInGame(true);
	}
}

void ANeedle::AttachMeshToPawn()
{
	Super::AttachMeshToPawn();
	if (MyPawn)
	{
		IgnoredActors.Add(MyPawn);
		FName AttachPoint = MyPawn->GetWeaponAttachPoint();
		USkeletalMeshComponent* PawnMesh1p = MyPawn->GetSpecificPawnMesh();
		if (PawnMesh1p != nullptr)
		{
			NeedleMesh->SetHiddenInGame(false);
			NeedleMesh->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
		}
	}
}

void ANeedle::ResetOrientation()
{
	Super::ResetOrientation();
}