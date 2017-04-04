// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Character/Guns/BaseGun.h"
#include "AssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AAssaultRifle : public ABaseGun
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		USceneComponent* TheRoot;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		class USkeletalMeshComponent* HarryAssaultRifle;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		class UStaticMeshComponent* Magazine;

	UPROPERTY(EditDefaultsOnly, Category = "C++ Gun")
		class URaycastComponent* RaycastComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		class UCameraComponent* OwningCamera;

	void SpawnVisualEffects();

public:
	// Sets default values for this actor's properties
	AAssaultRifle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Set as main weapon, and make it visible */
	virtual void WieldAndActivate() override;

	/** Set as secondary weapon, and make it invisible */
	virtual void SheathAndDeactivate() override;

	FORCEINLINE void SetOwningCamera(class UCameraComponent* _Cam) { OwningCamera = _Cam; }

	FORCEINLINE UCameraComponent* GetOwningCamera() const { return OwningCamera; }

	virtual void ResetOrientation() override;

	virtual void Shoot() override;

	virtual void HideGun() override;

	virtual void UnHideGun() override;

	USkeletalMeshComponent* GetGunMesh() const override;
protected:
	


	virtual void AttachMeshToPawn() override;

protected:
	
	
};
