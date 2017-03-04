// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Character/Guns/PlasmaRifle.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/Misc/ChildProjectiles/PlasmaGunProjectile.h"
#include "Headers/CustomComponents/RaycastComponent.h"
#include "Headers/AIEnemies/QuackAIPawn.h"
#include "Headers/Boss/QuackBoss.h"
#include "Headers/Boss/Armour/QuackArmourPin.h"

APlasmaRifle::APlasmaRifle()
{
	TheRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));
	RootComponent = TheRoot;

	HarryPlasmaGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HarryPlasmaGun"));
	HarryPlasmaGun->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	HarryPlasmaGun->bReceivesDecals = false;
	HarryPlasmaGun->CastShadow = false;
	HarryPlasmaGun->SetupAttachment(TheRoot);

	LaserParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserPS"));
	LaserParticleSystemComp->SetupAttachment(HarryPlasmaGun);

	RaycastComponent = CreateDefaultSubobject<URaycastComponent>(TEXT("RaycastComp"));

	Ammo = 100;
	ReservedAmmo = 1000;
	GunDamage = 2;
	FireRate = 0.1f;
	bIsFiring = false;
	bIsFullyAutomatic = true;
	bIsInUseUnableToSwap = false;

}

void APlasmaRifle::BeginPlay()
{
	Super::BeginPlay();
	if(LaserParticleSystemComp != nullptr && HarryPlasmaGun != nullptr)
		LaserParticleSystemComp->AttachToComponent(HarryPlasmaGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LaserSocket"));

}

void APlasmaRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFiring)
	{
		if (RaycastComponent != nullptr && LaserParticleSystemComp != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Plasma Fired"));
			// ToDo: Make New Shake for Plasma coz it will be different
			//PlayFeedbackShake();
			// Any visuals ?
			// Can use timer for audio.. but not important at the moment
			//PlayFeedbackAudio(MyPawn->GetSpecifcPawnMesh());
			DecreaseAmmo(AmmoDecayPerSecond * DeltaTime, MyPawn->GetSpecifcPawnMesh());
			EnableBeam();
			// Projectile logic for hitting a target
			// Note: Do here, or Within Prj class
			// Delegate / Callback or ???

			//UE_LOG(LogTemp, Warning, TEXT("Assault Rifle Fired"));
			FHitResult Hit = RaycastComponent->RaycastLaser(LaserParticleSystemComp, IgnoredActors);
			AActor* HitActor = Hit.GetActor();
			if (HitActor != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("WTF am i hitting : %s"), *(HitActor->GetName()));
				LaserTargetLocation = Hit.Location;
				SetLaserSource();
				SetLaserEnd();
				//print("Object Hit");
				AQuackArmourPin* ArmourPin = Cast<AQuackArmourPin>(HitActor);
				if (ArmourPin != nullptr)
				{
					//UE_LOG(LogTemp, Warning, TEXT("I Hit a : %s"), *ArmourPin->GetName());
					if (ArmourPin->GetArmourValue() >= 0.0f)
					{
						//UE_LOG(LogTemp, Warning, TEXT("%s , Armour: %f"), *ArmourPin->GetName(), ArmourPin->GetArmourValue());
						ArmourPin->DecreaseArmourValue(GunDamage);
					}
				}
				AQuackBoss* Boss = Cast<AQuackBoss>(HitActor);
				AQuackAIPawn* Enemy = Cast<AQuackAIPawn>(HitActor);
				if (Boss != nullptr)
				{
					// Call take damage on the boss
					Boss->SufferDamage(GunDamage * DeltaTime);
					Boss->ShouldEnterHealingPhase();
					//if (HitEffect != nullptr)
					//{
					//	UGameplayStatics::SpawnEmitterAtLocation(World, HitEffect, Hit.Location);
					//}
				}
				else if (Enemy != nullptr)
				{
					Enemy->TakeDamages(GunDamage * DeltaTime);
					//if (HitEffect != nullptr)
					//{
					//	UGameplayStatics::SpawnEmitterAtLocation(World, HitEffect, Hit.Location);
					//}
				}
			}
			else
			{
				//				LaserTargetLocation = LaserParticleSystemComp->GetComponentLocation() * (LaserParticleSystemComp->GetForwardVector() * RaycastComponent->GetRayLength());
				LaserTargetLocation = LaserParticleSystemComp->GetComponentLocation() * ((GetActorForwardVector() + FVector(0.0f,-90.0f,0.0f)) * RaycastComponent->GetRayLength());
				SetLaserSource();
				SetLaserEnd();
			}
		}
	}
	else
	{
		DisableBeam();
	}
}

USkeletalMeshComponent* APlasmaRifle::GetGunMesh() const
{
	return HarryPlasmaGun;
}

void APlasmaRifle::StopMuzzleFlash()
{
	Super::StopMuzzleFlash();
	//UE_LOG(LogTemp, Warning, TEXT("StopMuzzleFlash : Plasma"));
	DisableBeam();
	StopFiring();
}

void APlasmaRifle::Shoot()
{
	Super::Shoot();
	if (bIsReloading) 
	{
		StopMuzzleFlash(); 
		return;
	}
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	bIsFiring = true;
	//FVector CameraLoc;
	//FRotator CameraRot;
	//GetActorEyesViewPoint(CameraLoc, CameraRot);
	// Convert from camera to world space
	//FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(GunOffset);
	//FRotator MuzzleRotation = CameraRot;
	//MuzzleRotation.Pitch += 10.0f;

	//const FRotator SpawnRotation = MyPawn->GetControlRotation();
	//const FVector SpawnLocation = /*((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() :*/ MyPawn->GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
	//FActorSpawnParameters SpawnParams;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//if (PlasmaProjectileClass == nullptr) return;
	//APlasmaGunProjectile* PlasmaBall = World->SpawnActor<APlasmaGunProjectile>(PlasmaProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	//if (PlasmaBall != nullptr)
	//{

	// Above is the Projectile Version - if we want a ball system, as opposed to a beam one
	
		
	//}
}

void APlasmaRifle::StopFiring()
{
	bIsFiring = false;
	// might not need this call
	// but was bugfixing so ye why not
	DisableBeam();
}

void APlasmaRifle::SetLaserSource()
{
	if (LaserParticleSystemComp != nullptr)
	{
		LaserParticleSystemComp->SetBeamSourcePoint(0, LaserParticleSystemComp->GetComponentLocation(), 0);
	}
}

void APlasmaRifle::SetLaserEnd()
{
	if (LaserParticleSystemComp != nullptr)
	{
		LaserParticleSystemComp->SetBeamEndPoint(0, LaserTargetLocation);
		if (MuzzleFlashParticleSystem == nullptr && FireEffect != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MF Activated"));
			MuzzleFlashParticleSystem = UGameplayStatics::SpawnEmitterAttached(FireEffect, HarryPlasmaGun, SocketName);
			MuzzleFlashParticleSystem->SetWorldScale3D(FVector(0.75f));
		}
		if (MuzzleFlashParticleSystem != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MF Location updated: %s"), *LaserTargetLocation.ToString());
			MuzzleFlashParticleSystem->SetWorldLocation(LaserTargetLocation);
		}
	}
}

void APlasmaRifle::EnableBeam()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enable Beam"));

	if (LaserParticleSystemComp != nullptr)
	{
		if (LaserPS != nullptr)
		{
			LaserParticleSystemComp->SetTemplate(LaserPS);
			LaserParticleSystemComp->SetVisibility(true);
		}
	}
}

void APlasmaRifle::DisableBeam()
{
	//UE_LOG(LogTemp, Warning, TEXT("Disable Beam"));
	if (LaserParticleSystemComp != nullptr)
	{
		LaserParticleSystemComp->SetTemplate(EmptyPS);
		LaserParticleSystemComp->SetVisibility(false);
		//UE_LOG(LogTemp, Warning, TEXT("Disable Beam --- Set to false"));
	}
}
/** Set as main weapon, and make it visible */
void APlasmaRifle::WieldAndActivate()
{
	Super::WieldAndActivate();

	bIsActive = true;
	if (HarryPlasmaGun != nullptr)
	{
		HarryPlasmaGun->SetHiddenInGame(false);
	}
}

/** Set as secondary weapon, and make it invisible */
void APlasmaRifle::SheathAndDeactivate()
{
	Super::SheathAndDeactivate();

	bIsActive = false;
	DisableBeam();
	StopFiring();
	if (HarryPlasmaGun != nullptr)
	{
		HarryPlasmaGun->SetHiddenInGame(true);
		
	}
}

void APlasmaRifle::AttachMeshToPawn()
{
	Super::AttachMeshToPawn();
	if (MyPawn)
	{
		IgnoredActors.Add(MyPawn);

		UE_LOG(LogTemp, Warning, TEXT("PlasmaAttachedToPawn"));
		FName AttachPoint = MyPawn->GetWeaponAttachPoint();
		USkeletalMeshComponent* PawnMesh1p = MyPawn->GetSpecifcPawnMesh();
		if (PawnMesh1p != nullptr)
		{
			HarryPlasmaGun->SetHiddenInGame(false);
			HarryPlasmaGun->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
		}
	}
}

void APlasmaRifle::ResetOrientation()
{
	// Overridden
	Super::ResetOrientation();
	if (HarryPlasmaGun != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PR Orientated"));
		HarryPlasmaGun->SetRelativeRotation(FRotator(0.0f, 7.0f, 116.0f));
	}
}