// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Character/Guns/AssaultRifle.h"
#include "Headers/CustomComponents/RaycastComponent.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/AIEnemies/QuackAIPawn.h"
#include "Headers/Boss/QuackBoss.h"
#include "Headers/Boss/Armour/QuackArmourPin.h"

AAssaultRifle::AAssaultRifle()
{
	TheRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));
	RootComponent = TheRoot;

	HarryAssaultRifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HarryAssaultRifle"));
	HarryAssaultRifle->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	HarryAssaultRifle->bReceivesDecals = false;
	HarryAssaultRifle->CastShadow = false;
	HarryAssaultRifle->SetupAttachment(TheRoot);

	Magazine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));
	Magazine->SetupAttachment(HarryAssaultRifle);

	RaycastComponent = CreateDefaultSubobject<URaycastComponent>(TEXT("RaycastComp"));

	bIsFullyAutomatic = true;
	bIsInUseUnableToSwap = false;
}


void AAssaultRifle::BeginPlay()
{
	Super::BeginPlay();


}

void AAssaultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Magazine != nullptr)
	{
		if (MyPawn != nullptr)
		{
			if (MyPawn->GetSpecificPawnMesh() != nullptr)
			{
				if (bIsReloading)
				{
					Magazine->AttachToComponent(MyPawn->GetSpecificPawnMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MagazineSocket"));
				}
				else
				{
					Magazine->AttachToComponent(HarryAssaultRifle, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MagSocket"));
				}
			}
		}
	}

}

USkeletalMeshComponent* AAssaultRifle::GetGunMesh() const
{
	return HarryAssaultRifle;
}

void AAssaultRifle::SpawnVisualEffects()
{
	UWorld* const World = GetWorld();
	if (World == nullptr)return;
	if (FireEffect != nullptr && MuzzleFlashParticleSystem == nullptr)
	{
		//const FRotator SpawnRotation = GetControlRotation();
		// Spawn Looped Muzzle Flash
		MuzzleFlashParticleSystem = UGameplayStatics::SpawnEmitterAttached(FireEffect, HarryAssaultRifle, SocketName);
		MuzzleFlashParticleSystem->SetWorldScale3D(FVector(0.75f));
	}

	if (FireTrailEffect != nullptr)
	{
		//const FVector Origin = FP_Gun->GetMuzzleLocation();

		// THIS IS IN THE ARMS SKELETON BY HIS RIGHT HAND
		// NEW ARMS, ADD SOCKET ETC
		const FVector Origin = HarryAssaultRifle->GetSocketLocation(FName("GripPoint"));
		UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(World, FireTrailEffect, Origin);
		if (TrailPSC && OwningCamera != nullptr)
		{
			FVector EndTrace = OwningCamera->GetComponentLocation() + (OwningCamera->GetForwardVector() * 5000.0f);
			TrailPSC->SetVectorParameter(FName(TEXT("ShockBeamEnd")), EndTrace);
		}
	}
}

void AAssaultRifle::Shoot()
{
	Super::Shoot();
	if (bIsReloading)
	{
		StopMuzzleFlash();
		return;
	}

	if (Ammo <= 0) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		PlayFeedbackShake();
		SpawnVisualEffects();
		PlayFeedbackAudio(MyPawn->GetSpecificPawnMesh());
		DecreaseAmmo(1, MyPawn->GetSpecificPawnMesh());

		if (RaycastComponent != nullptr)
		{
			if (OwningCamera != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Assault Rifle Fired"));
				FHitResult Hit = RaycastComponent->Raycast(OwningCamera, IgnoredActors);
				
				AActor* HitActor = Hit.GetActor();
				if (HitActor != nullptr)
				{
					//UE_LOG(LogTemp, Warning, TEXT("I Hit Component : %s"), *Hit.Component.Get()->GetName());

					//UE_LOG(LogTemp, Warning, TEXT("I Hit a : %s"), *HitActor->GetName());
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
	}
}

/** Set as main weapon, and make it visible */
void AAssaultRifle::WieldAndActivate()
{
	Super::WieldAndActivate();

	bIsActive = true;
	if (HarryAssaultRifle != nullptr)
	{
		HarryAssaultRifle->SetHiddenInGame(false);
		Magazine->SetHiddenInGame(false);
	}
}

/** Set as secondary weapon, and make it invisible */
void AAssaultRifle::SheathAndDeactivate()
{
	Super::SheathAndDeactivate();

	bIsActive = false;
	if (HarryAssaultRifle != nullptr)
	{
		HarryAssaultRifle->SetHiddenInGame(true);
		Magazine->SetHiddenInGame(true);
	}
	StopMuzzleFlash();
}

void AAssaultRifle::AttachMeshToPawn()
{
	Super::AttachMeshToPawn();
	if (MyPawn)
	{
		IgnoredActors.Add(MyPawn);
		FName AttachPoint = MyPawn->GetWeaponAttachPoint();
		USkeletalMeshComponent* PawnMesh1p = MyPawn->GetSpecificPawnMesh();
		if (PawnMesh1p != nullptr)
		{
			HarryAssaultRifle->SetHiddenInGame(false);
			//HarryAssaultRifle->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
			HarryAssaultRifle->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
			DisabledEffect->AttachToComponent(HarryAssaultRifle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void AAssaultRifle::ResetOrientation()
{
	// Overridden
	// Deprecated
	Super::ResetOrientation();
	if (HarryAssaultRifle != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AR Orientated"));
		HarryAssaultRifle->SetRelativeRotation(FRotator(0.0f, 7.0f, 116.0f));
	}
}