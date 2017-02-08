// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIAttack.h"
#include "Headers/AIEnemies/QuackAIController.h"
#include "Headers/AIEnemies/QuackAIPawn.h"

EBTNodeResult::Type UQuackAIAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AQuackAIController* MyController = Cast<AQuackAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	AQuackAIPawn* TempPawn = Cast<AQuackAIPawn>(MyController->GetPawn());
	if (TempPawn != nullptr)
	{
		TempPawn->Attack();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}


