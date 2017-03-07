// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Character/Guns/BurstRifle.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/CustomComponents/RaycastComponent.h"
#include "Headers/AIEnemies/QuackAIPawn.h"
#include "Headers/Boss/QuackBoss.h"
#include "Headers/Boss/Armour/QuackArmourPin.h"

ABurstRifle::ABurstRifle()
{
	TheRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));
	RootComponent = TheRoot;

	HarryLaserGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HarryLaserGun"));
	HarryLaserGun->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	HarryLaserGun->bReceivesDecals = false;
	HarryLaserGun->CastShadow = false;
	HarryLaserGun->SetupAttachment(TheRoot);

	LaserParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserPS"));
	LaserParticleSystemComp->SetWorldScale3D(FVector(0.8f));
	LaserParticleSystemComp->SetRelativeLocation(FVector((0.0f, 50.0f, 5.0f)));
	LaserParticleSystemComp->SetupAttachment(HarryLaserGun);

	RaycastComponent = CreateDefaultSubobject<URaycastComponent>(TEXT("RaycastComp"));

	Ammo = 100;
	ReservedAmmo = 1000;
	GunDamage = 2;
	FireRate = 0.1f;
	bIsFiring = false;
	bIsFullyAutomatic = false;
}

USkeletalMeshComponent* ABurstRifle::GetGunMesh() const
{
	return HarryLaserGun;
}

void ABurstRifle::BeginPlay()
{
	Super::BeginPlay();
}

void ABurstRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABurstRifle::StopMuzzleFlash()
{
	Super::StopMuzzleFlash();
	//UE_LOG(LogTemp, Warning, TEXT("StopMuzzleFlash Called"));
	if (LaserParticleSystemComp != nullptr && bShootingInProcess)
	{
		LaserParticleSystemComp->DeactivateSystem();
		//LaserParticleSystemComp = nullptr;
	}
	EndLaserDuration();

}

void ABurstRifle::Shoot()
{
	Super::Shoot();
	UWorld* const World = GetWorld();
	if (bIsReloading || Ammo <= 0.0f)
	{
		StopMuzzleFlash();
		//UE_LOG(LogTemp, Warning, TEXT("Tried to Shoot but was shut down"));
		return;
	}
	if (bShootingInProcess)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Tried to Shoot but shooting was in process"));
		// SEEMS TO GET STUCK SOMEHOW - NOT SURE WHY
		// THIS SHOULD UNCHECK THIS BOOL NATURALLY	
		FTimerHandle EndHandle2;
		if(World != nullptr)
			World->GetTimerManager().SetTimer(EndHandle2, this, &ABurstRifle::EndLaserDuration, ParticleLength, false);
		return;
	}
	bShootingInProcess = true;
	if (World == nullptr || LaserParticleSystem == nullptr || MyPawn->GetSpecificPawnMesh() == nullptr) return;
	if (LaserParticleSystemComp == nullptr) return;
	LaserParticleSystemComp = UGameplayStatics::SpawnEmitterAttached(LaserParticleSystem, HarryLaserGun, MuzzleAttachPoint);// , ((FVector)(ForceInit)), FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	FTimerHandle RayHandle;
	FTimerHandle EndHandle;
	World->GetTimerManager().SetTimer(EndHandle, this, &ABurstRifle::EndLaserDuration, ParticleLength, false);
	World->GetTimerManager().SetTimer(RayHandle, this, &ABurstRifle::FireRay, (ParticleLength / 2), false);

	//PlayFeedbackShake();
	//PlayFeedbackAudio(MyPawn->GetSpecificPawnMesh());
}

void ABurstRifle::FireRay()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (RaycastComponent != nullptr)
	{
		bIsInUseUnableToSwap = true;
		bShootingInProcess = true;
		FTimerHandle AmmoHandle;
		World->GetTimerManager().SetTimer(AmmoHandle, this, &ABurstRifle::SubtractAmmo, ParticleLength / 2, false);
		FHitResult Hit = RaycastComponent->Raycast(MyPawn->GetFirstPersonCameraComponent(), IgnoredActors);
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			AQuackArmourPin* ArmourPin = Cast<AQuackArmourPin>(HitActor);
			if (ArmourPin != nullptr)
			{
				if (ArmourPin->GetArmourValue() >= 0.0f)
				{
					//UE_LOG(LogTemp, Warning, TEXT("%s , Armour: %f"), *ArmourPin->GetName(), ArmourPin->GetArmourValue());
					ArmourPin->DecreaseArmourValue(GunDamage);
					ArmourPin->White();
					//ArmourPin->Setvectorparam
					if (HitEffect != nullptr)
					{
						UGameplayStatics::SpawnEmitterAtLocation(World, HitEffect, Hit.Location);
					}
				}
			}
			//print("Object Hit");
			AQuackBoss* Boss = Cast<AQuackBoss>(HitActor);
			AQuackAIPawn* Enemy = Cast<AQuackAIPawn>(HitActor);
			if (Boss != nullptr)
			{
				// Call take damage on the boss
				Boss->SufferDamage(GunDamage);
				Boss->ShouldEnterHealingPhase();
				if (HitEffect != nullptr)
				{
					UGameplayStatics::SpawnEmitterAtLocation(World, HitEffect, Hit.Location);
				}
			}
			else if (Enemy != nullptr)
			{
				Enemy->TakeDamages(GunDamage);
				if (HitEffect != nullptr)
				{
					UGameplayStatics::SpawnEmitterAtLocation(World, HitEffect, Hit.Location);
				}
			}
		}
	}
}

void ABurstRifle::SubtractAmmo()
{
	if (MyPawn->GetSpecificPawnMesh() != nullptr)
	{
		DecreaseAmmo(1, MyPawn->GetSpecificPawnMesh());
		EndLaserDuration();
	}
}

void ABurstRifle::EndLaserDuration()
{
	bShootingInProcess = false;
	bIsInUseUnableToSwap = false;
	//UE_LOG(LogTemp, Warning, TEXT("Shootinprocess: %s, UnableToSwap: %s"), bShootingInProcess ? TEXT("true") : TEXT("False"), bIsInUseUnableToSwap ? TEXT("true") : TEXT("false"));
}

/** Set as main weapon, and make it visible */
void ABurstRifle::WieldAndActivate()
{
	Super::WieldAndActivate();

	bIsInUseUnableToSwap = false;
	bIsActive = true;
	EndLaserDuration();
	if (HarryLaserGun != nullptr)
	{
		HarryLaserGun->SetHiddenInGame(false);
	}
}

/** Set as secondary weapon, and make it invisible */
void ABurstRifle::SheathAndDeactivate()
{
	Super::SheathAndDeactivate();

	bIsActive = false;
	bIsInUseUnableToSwap = false;
	bShootingInProcess = false;
	// EndLaserDuration seems to do nothing : this is tilting me
	EndLaserDuration();
	UE_LOG(LogTemp, Warning, TEXT("Burst Rifle Sheathed: %s"), bIsInUseUnableToSwap ? TEXT("InUse") : TEXT("NotInUse"));
	if (HarryLaserGun != nullptr)
	{
		HarryLaserGun->SetHiddenInGame(true);

	}
}

void ABurstRifle::AttachMeshToPawn()
{
	Super::AttachMeshToPawn();
	if (MyPawn)
	{
		IgnoredActors.Add(MyPawn);
		FName AttachPoint = MyPawn->GetWeaponAttachPointLaser();
		USkeletalMeshComponent* PawnMesh1p = MyPawn->GetSpecificPawnMesh();
		if (PawnMesh1p != nullptr)
		{
			HarryLaserGun->SetHiddenInGame(false);
			//HarryLaserGun->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
			HarryLaserGun->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
			UE_LOG(LogTemp, Warning, TEXT("BurstRifle, should go to attachpoint"));
		}
	}
}

void ABurstRifle::ResetOrientation()
{
	// Overridden
	Super::ResetOrientation();
	if (HarryLaserGun != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PR Orientated"));
		HarryLaserGun->SetRelativeRotation(FRotator(0.0f, 7.0f, 116.0f));
	}
}