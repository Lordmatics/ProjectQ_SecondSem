// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Character/Guns/BaseGun.h"
#include "Headers/Character/QuackCharacter.h"
#include "Animation/AnimInstance.h"

// Sets default values
ABaseGun::ABaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGun::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABaseGun::Shoot()
{
}

/** Set as main weapon, and make it visible */
void ABaseGun::WieldAndActivate()
{
	bIsActive = true;
}

/** Set as secondary weapon, and make it invisible */
void ABaseGun::SheathAndDeactivate()
{
	bIsActive = false;
	bIsReloading = false;
	StopMuzzleFlash();
}

void ABaseGun::SetOwningPawn(AQuackCharacter* NewOwner)
{
	if (NewOwner != nullptr)
	{
		if (MyPawn != NewOwner)
		{
			UE_LOG(LogTemp, Warning, TEXT("GunBase OwningPawn Set"));

			Instigator = NewOwner;
			MyPawn = NewOwner;
			SetOwner(NewOwner);
			AttachMeshToPawn();
		}
	}
}

void ABaseGun::AttachMeshToPawn()
{
	// Overridden
}

void ABaseGun::ResetOrientation()
{
	// Overridden
}

// SHOOTING AND WEAPON LOGIC
void ABaseGun::DecreaseAmmo(float Amount, USkeletalMeshComponent* PlayerHands)
{
	Ammo -= Amount;
	Ammo = FMath::Clamp(Ammo, 0.0f, (float)MaxClipSize);
	if (Ammo <= 0.0f)
	{
		Reload(PlayerHands);
	}
}

void ABaseGun::IncreaseAmmo(float Amount)
{
	Ammo += Amount;
	Ammo = FMath::Clamp(Ammo, 0.0f, (float)MaxClipSize);
	CheckForMaxClip();
}

USkeletalMeshComponent* ABaseGun::GetGunMesh() const
{
	return nullptr;
}

// If you dont have a full clip
// Calculate how much ammo
// Your reserve can provide
// Then obtain it
float ABaseGun::Reload(USkeletalMeshComponent* PlayerHands)
{
	UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %f"), Ammo);

	if (FMath::FloorToInt(Ammo) < MaxClipSize && !bIsReloading)
	{
		// try and play a firing animation if specified
		if (ReloadAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = PlayerHands->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				StopMuzzleFlash();
				float Duration = AnimInstance->Montage_Play(ReloadAnimation, 1.f);
				bIsReloading = true;
				UWorld* const World = GetWorld();
				FTimerHandle ReloadHandle;
				//UE_LOG(LogTemp, Warning, TEXT("Reload Should take : %f"), Duration);
				World->GetTimerManager().SetTimer(ReloadHandle, this, &ABaseGun::FinishReload, Duration);
				return Duration;
			}
		}
	}
	return 0.0f;
}

void ABaseGun::FinishReload()
{
	bIsReloading = false;
	int RequiredAmmo = MaxClipSize - FMath::FloorToInt(Ammo);
	UE_LOG(LogTemp, Warning, TEXT("Required Ammo: %d"), RequiredAmmo);
	UE_LOG(LogTemp, Warning, TEXT("Reserved Ammo: %f"), ReservedAmmo);
	if (ReservedAmmo >= RequiredAmmo)
	{
		Ammo += RequiredAmmo;
		ReservedAmmo -= RequiredAmmo;
		Ammo = FMath::Clamp(Ammo, 0.0f, (float)MaxClipSize);
	}
	else
	{
		Ammo += ReservedAmmo;
		Ammo = FMath::Clamp(Ammo, 0.0f, (float)MaxClipSize);
		ReservedAmmo = 0.0f;
	}
}

// If you pick up ammo
// That exceeds your clip size
// You wont lose it
// Instead it gets added to your reserve
void ABaseGun::CheckForMaxClip()
{
	if (Ammo >= MaxClipSize)
	{
		ReservedAmmo += (Ammo - MaxClipSize);
		Ammo = MaxClipSize;
	}
}

void ABaseGun::PlayFeedbackShake()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	APlayerController* PC = World->GetFirstPlayerController();
	if (PC != nullptr)
	{
		if (CameraShake != nullptr)
		{
			PC->ClientPlayCameraShake(CameraShake, 1.0f);
		}
		if (Feedback != nullptr)
		{
			PC->ClientPlayForceFeedback(Feedback, false, "Weapon");
		}
	}
}

void ABaseGun::PlayFeedbackAudio(USkeletalMeshComponent* PlayerHands)
{
	if (PlayerHands == nullptr) return;
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), GunShootVolume);
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = PlayerHands->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ABaseGun::StopMuzzleFlash()
{
	if (MuzzleFlashParticleSystem != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MuzzleFlashDeactivated"));
		MuzzleFlashParticleSystem->DeactivateSystem();
		MuzzleFlashParticleSystem = nullptr;
	}
}

float ABaseGun::GetCurrentAmmo() const
{
	return Ammo;
}

float ABaseGun::GetMaxAmmo() const
{
	return ReservedAmmo;
}