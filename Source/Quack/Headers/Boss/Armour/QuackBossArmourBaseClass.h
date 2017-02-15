// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "QuackBossArmourBaseClass.generated.h"

UCLASS()
class QUACK_API AQuackBossArmourBaseClass : public AActor
{
	GENERATED_BODY()
protected:
	/** Root Component*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Armour")
		USceneComponent* MyRoot;

	/** Visible Display of the armour piece*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Armour")
		UStaticMeshComponent* ArmourPiece;

	/** Certain Pieces of armour will have their own health, so each need their own triggers*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Armour")
		UBoxComponent* ArmourHitBox;

	UPROPERTY(EditAnywhere, Category = "C++ Armour")
		float ArmourValue = 100.0f;

	void Die();
public:	
	// Sets default values for this actor's properties
	AQuackBossArmourBaseClass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FORCEINLINE void DecreaseArmourValue(float _Damage) { ArmourValue -= _Damage; 
	ArmourValue = FMath::Clamp(ArmourValue, 0.0f, 100.0f); }
	FORCEINLINE float GetArmourValue() const { return ArmourValue; }
	
	UPROPERTY(VisibleAnywhere, Category = "C++ Armour")
		uint32 bHasBeenDestroyed : 1;

	void ToggleHighlight(bool _Highlight);
};
