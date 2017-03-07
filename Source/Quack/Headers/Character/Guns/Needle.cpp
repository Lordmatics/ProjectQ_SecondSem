// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Needle.h"
#include "Headers/Character/QuackCharacter.h"
#include "Animation/AnimInstance.h"

ANeedle::ANeedle()
{
	TheRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));
	RootComponent = TheRoot;

	//NeedleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NeedleMesh"));
	//NeedleMesh->bReceivesDecals = false;
	//NeedleMesh->CastShadow = false;
	//NeedleMesh->SetupAttachment(RootComponent);

	NeedleSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NeedleSkeletalMesh"));
	NeedleSkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	NeedleSkeletalMesh->bReceivesDecals = false;
	NeedleSkeletalMesh->CastShadow = false;
	NeedleSkeletalMesh->SetHiddenInGame(true);
	NeedleSkeletalMesh->SetupAttachment(TheRoot);

	NeedleSkeletalMeshForAnimations = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NeedleSkeletalMeshForAnimations"));
	NeedleSkeletalMeshForAnimations->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	NeedleSkeletalMeshForAnimations->bReceivesDecals = false;
	NeedleSkeletalMeshForAnimations->CastShadow = false;
	NeedleSkeletalMeshForAnimations->SetupAttachment(TheRoot);

	bIsFullyAutomatic = true;
	bIsInUseUnableToSwap = false;
	bStabbing = false;
}

void ANeedle::BeginPlay()
{
	Super::BeginPlay();
	//if (MyPawn->GetSpecificPawnMesh() != nullptr)
	//{
	//	NeedleSkeletalMeshForAnimations->AttachToComponent(MyPawn->GetSpecificPawnMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("NeedleSocket"));	
	//}
	//AdjustVisibilities(false);
}

void ANeedle::Tick(float DeltaSeconds)
{
	
}

void ANeedle::Shoot()
{
	//PlayStabAnimation();
}

void ANeedle::PlayStabAnimation()
{
	// try and play a firing animation if specified
	if (NeedleStabAnimation != NULL && !bStabbing)
	{
		bStabbing = true;
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = MyPawn->GetSpecificPawnMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			float Duration = AnimInstance->Montage_Play(NeedleStabAnimation, 1.f);
			//AdjustVisibilities(true);
			UWorld* const World = GetWorld();
			if (World == nullptr) return;
			FTimerHandle TempHandle;
			World->GetTimerManager().SetTimer(TempHandle, this, &ANeedle::EndStab, Duration, false);
		}
	}
}

// Deprecated - due to idle animation, now only need one
void ANeedle::AdjustVisibilities(bool bForAnimations)
{
	if (NeedleSkeletalMesh != nullptr && NeedleSkeletalMeshForAnimations != nullptr)
	{
		NeedleSkeletalMesh->SetHiddenInGame(bForAnimations);
		NeedleSkeletalMeshForAnimations->SetHiddenInGame(!bForAnimations);
	}
}

void ANeedle::EndStab()
{
	bStabbing = false;
	//AdjustVisibilities(false);
}

void ANeedle::WieldAndActivate()
{
	Super::WieldAndActivate();
	bIsActive = true;
	//if (NeedleSkeletalMesh != nullptr)
	//{
	//	NeedleSkeletalMesh->SetHiddenInGame(false);
	//}
	if (NeedleSkeletalMeshForAnimations != nullptr)
	{
		NeedleSkeletalMeshForAnimations->SetHiddenInGame(false);
	}
}

void ANeedle::SheathAndDeactivate()
{
	Super::SheathAndDeactivate();
	bIsActive = false;
	bStabbing = false;
	//if (NeedleSkeletalMesh != nullptr)
	//{
	//	NeedleSkeletalMesh->SetHiddenInGame(true);
	//}
	if (NeedleSkeletalMeshForAnimations != nullptr)
	{
		NeedleSkeletalMeshForAnimations->SetHiddenInGame(true);
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
			if (NeedleSkeletalMeshForAnimations != nullptr)
			{
				NeedleSkeletalMeshForAnimations->SetHiddenInGame(false);
			}
			//NeedleSkeletalMesh->SetHiddenInGame(false);
			//NeedleSkeletalMesh->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
			NeedleSkeletalMeshForAnimations->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("NeedleSocket"));
		}
	}
}

void ANeedle::ResetOrientation()
{
	Super::ResetOrientation();
}