// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Character/Guns/BaseGun.h"
#include "PlasmaRifle.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API APlasmaRifle : public ABaseGun
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		USceneComponent* TheRoot;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		class USkeletalMeshComponent* HarryPlasmaGun;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		UParticleSystemComponent* LaserParticleSystemComp;

	UPROPERTY(EditAnywhere, Category = "C++ Gun")
		UParticleSystem* LaserPS;

	UPROPERTY(EditAnywhere, Category = "C++ Gun")
		UParticleSystem* EmptyPS;

	UPROPERTY(EditDefaultsOnly, Category = "C++ Gun")
		class URaycastComponent* RaycastComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		FVector LaserTargetLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		uint32 bIsFiring : 1;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		float AmmoDecayPerSecond = 5.0f;

	void SetLaserSource();

	void SetLaserEnd();

	void StopFiring();

public:
	// Sets default values for this actor's properties
	APlasmaRifle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Set as main weapon, and make it visible */
	virtual void WieldAndActivate();

	/** Set as secondary weapon, and make it invisible */
	virtual void SheathAndDeactivate();

	virtual void ResetOrientation() override;

	virtual void Shoot() override;

	USkeletalMeshComponent* GetGunMesh() const override;

	virtual void DisableBeam();

	virtual void StopMuzzleFlash() override;

protected:

	virtual void EnableBeam();


	UPROPERTY(EditAnywhere, Category = "C++ Gun")
		TSubclassOf<class APlasmaGunProjectile> PlasmaProjectileClass;

	virtual void AttachMeshToPawn() override;
protected:
	
	
};
