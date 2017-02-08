// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "EdQuackBoss.generated.h"

UCLASS()
class QUACK_API AEdQuackBoss : public APawn
{
	GENERATED_BODY()



public:
	// Sets default values for this actor's properties
	AEdQuackBoss();

	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MaxHealth;
	float CurrentHealth;

	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

public:


	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;

};
