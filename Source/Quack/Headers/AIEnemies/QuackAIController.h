// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "QuackAIController.generated.h"

/**
*
*/
UCLASS()
class QUACK_API AQuackAIController : public AAIController
{
	GENERATED_BODY()

		AQuackAIController();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		UAIPerceptionComponent* Perception;
	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;
	UBehaviorTreeComponent* Behaviour;
	UBlackboardComponent* Blackboard;

	UFUNCTION()
		void UpdatePerception(TArray<AActor*>ActorsInSight);

	UFUNCTION()
		virtual void Possess(class APawn* InPawn) override;


};
