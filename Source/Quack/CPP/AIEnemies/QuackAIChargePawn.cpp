// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIChargePawn.h"
#include "Headers/AIEnemies/QuackAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

//HE THINKS HE CAN FACE ME IN SOLO QUEUE?????? ME?????

// LUL

void AQuackAIChargePawn::Die()
{
	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AQuackAIChargePawn::DestroyThis, 2.0f, false);
	AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
	if (TempController != nullptr)
	{
		TempController->Blackboard->SetValueAsBool("Dead", true);
	}
}

void AQuackAIChargePawn::DestroyThis()
{
	Destroy();
}

bool AQuackAIChargePawn::GetDeath()
{
	return (CurrentHealth <= 0);
}