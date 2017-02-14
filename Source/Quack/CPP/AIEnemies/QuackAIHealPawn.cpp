// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIHealPawn.h"
#include "Headers/Misc/Pipe.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/AIEnemies/QuackAIController.h"


AQuackAIHealPawn::AQuackAIHealPawn()
{

}

void AQuackAIHealPawn::BeginPlay()
{
	Super::BeginPlay();

	AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
	if (TempController != nullptr)
	{
		if (Pipe != nullptr)
		{
			TempController->Blackboard->SetValueAsObject("Pipe", Pipe);
		}
	}
}

void AQuackAIHealPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsHealing)
	{
		if (Pipe != nullptr)
		{
			if (!Pipe->GetDescend())
			{
				CurrentHealth += DeltaTime * 20;
			}
			else
			{
				IsHealing = false;
				Invincible = false;
				AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
				if (TempController != nullptr)
				{
					TempController->Blackboard->SetValueAsBool("Healing", false);
					IsRunning = true;
				}
			}
		}
	}
}

void AQuackAIHealPawn::Attack()
{
	Heal();
}

void AQuackAIHealPawn::Heal()
{
	IsHealing = true;
	Invincible = true;
	IsRunning = false;
}

bool AQuackAIHealPawn::GetHealing()
{
	return IsHealing;
}

bool AQuackAIHealPawn::GetRunning()
{
	return IsRunning;
}

bool AQuackAIHealPawn::GetDeath()
{
	return (CurrentHealth <= 0);
}

void AQuackAIHealPawn::TakeDamages(float Damage)
{
	if (!Invincible) {
		Super::TakeDamages(Damage);
		if (CurrentHealth > 0 && CurrentHealth < MaxHealth / 2)
		{
			AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
			if (TempController != nullptr)
			{
				TempController->Blackboard->SetValueAsBool("Healing", true);
				IsRunning = true;
			}
		}
	}
}