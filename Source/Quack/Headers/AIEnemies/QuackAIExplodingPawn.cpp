// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIExplodingPawn.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/AIEnemies/QuackAIController.h"
#include "Components/DestructibleComponent.h"
#include "Headers/Managers/TutorialManager.h"

AQuackAIExplodingPawn::AQuackAIExplodingPawn()
{
	ExplodingMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	ExplodingMesh->OnComponentFracture.AddDynamic(this, &AQuackAIExplodingPawn::OnComponentFracture);
	ExplodingMesh->SetupAttachment(GetCapsuleComponent());
}

void AQuackAIExplodingPawn::BeginPlay()
{
	Super::BeginPlay();
	ExplodingMesh->SetVisibility(false);
}

void AQuackAIExplodingPawn::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Get Attacked"));
	AQuackCharacter* PlayerCharacter = Cast<AQuackCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter != nullptr)
	{
	//PlayerCharacter->GetCharacterMovement()->AddImpulse(GetActorLocation() - PlayerCharacter->GetActorLocation());
	PlayerCharacter->DecreaseHealth(Damage);
	GetMesh()->SetVisibility(false);
	//do attack stuff
	ExplodingMesh->SetVisibility(true);
	ExplodingMesh->ApplyRadiusDamage(100, ExplodingMesh->GetComponentLocation(), 20, 10000, true);
	if (TutorialManager != nullptr)
	{
		TutorialManager->WormDiedAt(WormID);
	}
	AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
	if (TempController != nullptr)
	{
		TempController->Blackboard->SetValueAsBool("Dead", true);
		//TempController->GetBrainComponent()->StopLogic(TEXT("Dead"));
	}
	//Destroy();	
	}
}

void AQuackAIExplodingPawn::OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection)
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ExplodingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AQuackAIExplodingPawn::DestroyThis, 2.0f, false);
	if (OnEnemyDestroyed.IsBound())
		OnEnemyDestroyed.Broadcast(this);
	if (OnEnemyDestroyedRoom.IsBound())
		OnEnemyDestroyedRoom.Broadcast(this);
}

void AQuackAIExplodingPawn::Die()
{
	if (TutorialManager != nullptr)
	{
		TutorialManager->WormDiedAt(WormID);

	}
	GetMesh()->SetVisibility(false);
	ExplodingMesh->SetVisibility(true);
	ExplodingMesh->ApplyRadiusDamage(100, ExplodingMesh->GetComponentLocation(), 20, 10000, true);
	AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
	if (TempController != nullptr)
	{
		//TempController->GetBrainComponent()->StopLogic(TEXT("Dead"));
		TempController->Blackboard->SetValueAsBool("Dead", true);
	}
}

void AQuackAIExplodingPawn::DestroyThis()
{
	Destroy();
}

