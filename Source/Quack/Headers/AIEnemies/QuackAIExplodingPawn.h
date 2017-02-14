// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/AIEnemies/QuackAIPawn.h"
#include "QuackAIExplodingPawn.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AQuackAIExplodingPawn : public AQuackAIPawn
{
	GENERATED_BODY()

		AQuackAIExplodingPawn();

		UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* ExplodingMesh;
	
	UFUNCTION()
		void OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection);

	virtual void Attack() override;
	
	virtual void Die() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void DestroyThis();

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class AQuackProjectile> Projectile;
};
