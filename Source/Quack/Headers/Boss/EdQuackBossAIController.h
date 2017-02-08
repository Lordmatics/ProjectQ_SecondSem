// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EdQuackBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API AEdQuackBossAIController : public AAIController
{
	GENERATED_BODY()

		AEdQuackBossAIController();	
public:
	UBehaviorTreeComponent* Behaviour;
	UBlackboardComponent* Blackboard;

	UFUNCTION()
	virtual void Possess(class APawn* InPawn) override;

};
