// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/AIEnemies/QuackAIPawn.h"
#include "QuackAIHealPawn.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AQuackAIHealPawn : public AQuackAIPawn
{
	GENERATED_BODY()

private:
	
	AQuackAIHealPawn();

	virtual void BeginPlay() override;

	virtual void Attack() override;

	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamages(float Damage) override;

	virtual void Die() override;

	void Heal();

	bool IsHealing;

	bool IsRunning;

	bool Invincible;

	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class APipe* Pipe;
	

public:

	UFUNCTION(BlueprintCallable, Category = "Anim")
		bool GetDeath();

	UFUNCTION(BlueprintCallable, Category = "Anim")
		bool GetHealing();

	UFUNCTION(BlueprintCallable, Category = "Anim")
		bool GetRunning();
};
