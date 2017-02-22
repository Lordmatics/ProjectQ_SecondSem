// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/AIEnemies/QuackAIPawn.h"
#include "QuackAIChargePawn.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AQuackAIChargePawn : public AQuackAIPawn
{
	GENERATED_BODY()
	
		virtual void Die() override;

	virtual void Attack() override;

	bool Attacking;

	UFUNCTION()
		void EndStun();

	AQuackAIChargePawn();

	UFUNCTION()
	void DealDamage(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void DestroyThis();

	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int WalkSpeed;
	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int RunSpeed;

	bool Stunned;

public:

	UFUNCTION(BlueprintCallable, Category = "Anim")
		bool GetDeath();

	UFUNCTION(BlueprintCallable, Category = "Anim")
		bool GetStunned();

	UFUNCTION(BlueprintCallable, Category = "Anim")
		bool GetAttacking();
	
};
