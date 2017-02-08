// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Boss/EdQuackBossAIController.h"
#include "Headers/Boss/EdQuackBoss.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AEdQuackBossAIController::AEdQuackBossAIController()
{
	Behaviour = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AEdQuackBossAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	AEdQuackBoss* AIBoss = Cast<AEdQuackBoss>(InPawn);
	if (AIBoss != nullptr)
	{
		if (AIBoss->BehaviorTree != nullptr)
		{
			Blackboard->InitializeBlackboard(*AIBoss->BehaviorTree->BlackboardAsset);
			Behaviour->StartTree(*AIBoss->BehaviorTree);
		}
	}
}


