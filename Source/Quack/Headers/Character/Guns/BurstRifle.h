// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Character/Guns/BaseGun.h"
#include "BurstRifle.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API ABurstRifle : public ABaseGun
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, Category = "C++ Gun")
	uint32 bShootingInProcess : 1;

	UFUNCTION()
		void SubtractAmmo();

	UFUNCTION()
		void FireRay();

	UFUNCTION()
		void EndLaserDuration();

	UPROPERTY(EditAnywhere, Category = "C++ Gun")
		float ParticleLength = 2.3f;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		USceneComponent* TheRoot;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		class USkeletalMeshComponent* HarryLaserGun;

	UPROPERTY(EditAnywhere, Category = "C++ Gun")
		UParticleSystemComponent* LaserParticleSystemComp;

	UPROPERTY(EditAnywhere, Category = "C++ Gun")
		UParticleSystem* LaserParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category = "C++ Gun")
		class URaycastComponent* RaycastComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		uint32 bIsFiring : 1;

	void Charge();

	uint32 bCharging : 1;
public:
	// Sets default values for this actor's properties
	ABurstRifle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Set as main weapon, and make it visible */
	virtual void WieldAndActivate() override;

	/** Set as secondary weapon, and make it invisible */
	virtual void SheathAndDeactivate() override;

	virtual void ResetOrientation() override;

	virtual void Shoot() override;

	USkeletalMeshComponent* GetGunMesh() const override;

	virtual void StopMuzzleFlash() override;

	void ResetLaserCharge();

	void Blast();

protected:

	virtual void AttachMeshToPawn() override;
protected:

	
	
};
