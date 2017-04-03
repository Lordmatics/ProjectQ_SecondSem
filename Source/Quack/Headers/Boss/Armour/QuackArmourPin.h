// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Boss/Armour/QuackBossArmourBaseClass.h"
#include "QuackArmourPin.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AQuackArmourPin : public AQuackBossArmourBaseClass
{
	GENERATED_BODY()
	
private:
	AQuackArmourPin();

	UFUNCTION()
		void Red();

	UPROPERTY(EditAnywhere, Category = "Emissive Pin")
		float TurnBackToRedDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Emissive Pin")
		float DissolveRate = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Emissive Pin")
		float Delay = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Emissive Pin")
		float Alpha;

	UPROPERTY(EditAnywhere, Category = "Emsisive Pin")
		uint32 bShouldFadeIn : 1;

private:

	UFUNCTION()
		void UpdateDissolve();
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, Category = "Emissive Pin")
		UMaterialInstanceDynamic* PinMaterial;

	UPROPERTY(EditAnywhere, Category = "Emissive Pin")
		UMaterialInstanceDynamic* EmissivePin;

	void White();

	FORCEINLINE bool IsFadingIn() const { return bShouldFadeIn; }
	FORCEINLINE void SetFade(bool _Dissolve) { bShouldFadeIn = _Dissolve; }

	FORCEINLINE float GetAlpha() const { return Alpha; }
	void SetAlpha(float _Alpha);
protected:
	
	
};
