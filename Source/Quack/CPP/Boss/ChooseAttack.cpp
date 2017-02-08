// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Boss/ChooseAttack.h"
#include "Headers/Boss/EdQuackBossAIController.h"

EBTNodeResult::Type UChooseAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEdQuackBossAIController* MyController = Cast<AEdQuackBossAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

