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
		// Pre-Assigned Pipe
		// Might want to add logic, to make this
		// More dynamic, depending on actual level design
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
			// Guessing, descend determines if pipe is drained or not
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

void AQuackAIHealPawn::TakeDamages(float _Damage)
{
	if (!Invincible) 
	{
		Super::TakeDamages(_Damage);
		if (CurrentHealth > 0.0f && CurrentHealth < MaxHealth / 2.0f)
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

void AQuackAIHealPawn::Die()
{
	if (OnEnemyDestroyed.IsBound())
		OnEnemyDestroyed.Broadcast(this);
	if (OnEnemyDestroyedRoom.IsBound())
		OnEnemyDestroyedRoom.Broadcast(this);
	Super::Die();
}