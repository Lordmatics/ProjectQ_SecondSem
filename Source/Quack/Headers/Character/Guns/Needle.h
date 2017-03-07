// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Character/Guns/BaseGun.h"
#include "Needle.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API ANeedle : public ABaseGun
{
	GENERATED_BODY()
	
		UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		USceneComponent* TheRoot;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		class UStaticMeshComponent* NeedleMesh;
	
public:

	ANeedle();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Set as main weapon, and make it visible */
	virtual void WieldAndActivate() override;

	/** Set as secondary weapon, and make it invisible */
	virtual void SheathAndDeactivate() override;

	virtual void ResetOrientation() override;

	virtual void Shoot() override;

protected:



	virtual void AttachMeshToPawn() override;
	
};
