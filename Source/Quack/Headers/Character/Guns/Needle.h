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

	///** Gun mesh: 1st person view (seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
	//	class UStaticMeshComponent* NeedleMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		class USkeletalMeshComponent* NeedleSkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Gun")
		class USkeletalMeshComponent* NeedleSkeletalMeshForAnimations;

	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* NeedleStabAnimation;

	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* NeedleRaiseAnimation;

	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* NeedleLowerAnimation;

	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* NeedleLookAtAnimation;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Stab")
		uint32 bStabbing : 1;

	UFUNCTION()
		void EndStab();
public:

	ANeedle();

	void PlayStabAnimation();

	void AdjustVisibilities(bool bForAnimations);

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Set as main weapon, and make it visible */
	virtual void WieldAndActivate() override;

	/** Set as secondary weapon, and make it invisible */
	virtual void SheathAndDeactivate() override;

	virtual void ResetOrientation() override;

	virtual void Shoot() override;

	// Forceinlines
	FORCEINLINE UAnimMontage* GetNeedleStabAnimation() const { return NeedleStabAnimation; }
	FORCEINLINE UAnimMontage* GetNeedleRaiseAnimation() const { return NeedleRaiseAnimation; }
	FORCEINLINE UAnimMontage* GetNeedleLowerAnimation() const { return NeedleLowerAnimation; }
	FORCEINLINE UAnimMontage* GetNeedleLookAtAnimation() const { return NeedleLookAtAnimation; }


protected:



	virtual void AttachMeshToPawn() override;
	
};
