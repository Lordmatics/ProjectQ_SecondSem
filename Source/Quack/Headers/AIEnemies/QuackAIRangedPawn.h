// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/AIEnemies/QuackAIPawn.h"
#include "QuackAIRangedPawn.generated.h"

/**
*
*/
UCLASS()
class QUACK_API AQuackAIRangedPawn : public AQuackAIPawn
{
	GENERATED_BODY()

		AQuackAIRangedPawn();

	virtual void Attack() override;

	virtual void BeginPlay() override;

	virtual void TakeDamages(float DamageIn) override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class AQuackProjectile> Projectile;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
		class UArrowComponent* ProjectileSpawn;

	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float AttackRange;

	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float FleeRange;

	FTimerHandle DelayTimer;

	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float FireDelay;

	void ClearTimer();

	bool CanFire;

};
