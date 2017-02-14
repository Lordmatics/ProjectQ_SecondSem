// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIChargePawn.h"
#include "Headers/AIEnemies/QuackAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Headers/CustomComponents/AnimationComponent.h"

//HE THINKS HE CAN FACE ME IN SOLO QUEUE?????? ME?????

// LUL

void AQuackAIChargePawn::Die()
{
	FTimerHandle DestroyTimer;
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	// Probably dont need this, unless something should happen inbetween it dying - and dissappearring
	World->GetTimerManager().SetTimer(DestroyTimer, this, &AQuackAIChargePawn::DestroyThis, 2.0f, false);
	AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
	if (TempController != nullptr)
	{
		TempController->Blackboard->SetValueAsBool("Dead", true);
	}
	if (OnEnemyDestroyed.IsBound())
		OnEnemyDestroyed.Broadcast(this);
	if (OnEnemyDestroyedRoom.IsBound())
		OnEnemyDestroyedRoom.Broadcast(this);
}

void AQuackAIChargePawn::DestroyThis()
{
	Destroy();
}

bool AQuackAIChargePawn::GetDeath()
{
	return (CurrentHealth <= 0);
}

void AQuackAIChargePawn::Attack()
{
	// Example of how to anim in code for Ed

	//if (AnimationComponent != nullptr)
	//{
	//	// int = 0, 1, 2
	//	int RandomAttack = FMath::RandRange(0, 2);
	//	float Duration = 1.0f;

	//	switch (RandomAttack)
	//	{
	//		case 0:
	//		{
	//			if(AnimationComponent->GetAttackAnimMontageA() != nullptr)
	//				Duration = PlayAnimMontage(AnimationComponent->GetAttackAnimMontageA(), 1.0f, NAME_None);
	//			break;
	//		}
	//		case 1:
	//		{
	//			if(AnimationComponent->GetAttackAnimMontageB() != nullptr)
	//				Duration = PlayAnimMontage(AnimationComponent->GetAttackAnimMontageB(), 1.0f, NAME_None);
	//			break;
	//		}
	//		case 2:
	//		{
	//			if(AnimationComponent->GetAttackAnimMontageC() != nullptr)
	//				Duration = PlayAnimMontage(AnimationComponent->GetAttackAnimMontageC(), 1.0f, NAME_None);
	//			break;
	//		}
	//		default:
	//		{
	//			if(AnimationComponent->GetAttackAnimMontageA() != nullptr)
	//				Duration = PlayAnimMontage(AnimationComponent->GetAttackAnimMontageA(), 1.0f, NAME_None);
	//			break;
	//		}
	//	}

	//	UWorld* const World = GetWorld();
	//	if (World != nullptr)
	//	{
	//		FTimerHandle AttackTimerHandle;
	//		// Change the called function to something else, if u want him to do something after his attack, otherwise
	//		// Dont need this
	//		World->GetTimerManager().SetTimer(AttackTimerHandle, this, &AQuackAIChargePawn::DestroyThis, Duration, false);
	//		// Do whatever after the attack
	//	}

	//}
	Attacking = true;
}

bool AQuackAIChargePawn::GetAttacking()
{
	return Attacking;
}