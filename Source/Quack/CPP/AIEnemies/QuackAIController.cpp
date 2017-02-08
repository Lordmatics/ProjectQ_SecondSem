// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIController.h"
#include "Headers/AIEnemies/QuackAIPawn.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AQuackAIController::AQuackAIController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	Behaviour = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	Perception->ConfigureSense(*SightConfig);
	Perception->ConfigureSense(*HearingConfig);
	Perception->SetDominantSense(SightConfig->GetSenseImplementation());
	Perception->OnPerceptionUpdated.AddDynamic(this, &AQuackAIController::UpdatePerception);
}

void AQuackAIController::UpdatePerception(TArray<AActor*>ActorsInSight)
{
	//UE_LOG(LogTemp, Warning, TEXT("I see you"));
	APawn* TempCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (TempCharacter != nullptr)
	{
		for (int i = 0; i < ActorsInSight.Num(); i++)
		{
			if (ActorsInSight[i] == TempCharacter)
			{
				Blackboard->SetValueAsObject("AttackTarget", TempCharacter);
			}
		}
	}
}

void AQuackAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	AQuackAIPawn* AICharacter = Cast<AQuackAIPawn>(InPawn);
	if (AICharacter != nullptr)
	{
		if (AICharacter->BehaviorTree != nullptr)
		{
			Blackboard->InitializeBlackboard(*AICharacter->BehaviorTree->BlackboardAsset);
			Behaviour->StartTree(*AICharacter->BehaviorTree);
		}
	}
}