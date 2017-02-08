// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/MinionFactoryComponent.h"
#include "Headers/AIEnemies/QuackAIPawn.h"

// Sets default values for this component's properties
UMinionFactoryComponent::UMinionFactoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMinionFactoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMinionFactoryComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UMinionFactoryComponent::SpawnMinionWaveA()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (SpawnPointsA.Num() == 0) return;
	if (EnemyFactoryA.Num() == 0) return;
	for (size_t i = 0; i < SpawnPointsA.Num(); i++)
	{
		int EnemyIndex = i > EnemyFactoryA.Num() - 1 ? 0 : FMath::RandRange(0, EnemyFactoryA.Num() - 1);
		AQuackAIPawn* Minion = World->SpawnActor<AQuackAIPawn>(EnemyFactoryA[EnemyIndex], SpawnPointsA[i]->GetActorLocation(), FRotator::ZeroRotator);
		if (Minion != nullptr)
		{
			Minion->OnEnemyDestroyed.AddDynamic(this, &UMinionFactoryComponent::RemoveFromListA);
			MinionArrayA.Add(Minion);
		}
	}
}

void UMinionFactoryComponent::SpawnMinionWaveB()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (SpawnPointsB.Num() == 0) return;
	if (EnemyFactoryB.Num() == 0) return;
	for (size_t i = 0; i < SpawnPointsB.Num(); i++)
	{
		int EnemyIndex = i > EnemyFactoryB.Num() - 1 ? 0 : FMath::RandRange(0, EnemyFactoryB.Num() - 1);
		AQuackAIPawn* Minion = World->SpawnActor<AQuackAIPawn>(EnemyFactoryB[EnemyIndex], SpawnPointsB[i]->GetActorLocation(), FRotator::ZeroRotator);
		if (Minion != nullptr)
		{
			Minion->OnEnemyDestroyed.AddDynamic(this, &UMinionFactoryComponent::RemoveFromListB);
			MinionArrayB.Add(Minion);
		}
	}
}

void UMinionFactoryComponent::RemoveFromListA(AQuackAIPawn* _Enemy)
{
	MinionArrayA.Remove(_Enemy);
	//if (MinionArrayA.Num() == 0 && MinionArrayB.Num() == 0)
	//{
	//}
}

void UMinionFactoryComponent::RemoveFromListB(AQuackAIPawn* _Enemy)
{
	MinionArrayB.Remove(_Enemy);
	//if (MinionArrayA.Num() == 0 && MinionArrayB.Num() == 0)
	//{
	//}
}

bool UMinionFactoryComponent::AreMinionsAlive()
{
	int A = MinionArrayA.Num();
	//UE_LOG(LogTemp, Warning, TEXT("Array A Length: %d"), A);
	int B = MinionArrayB.Num();
	//UE_LOG(LogTemp, Warning, TEXT("Array B Length: %d"), B);
	bool ShouldShieldBeUp = A + B > 0 ? true : false;
	//UE_LOG(LogTemp, Warning, TEXT("Array B Length: %s"), ShouldShieldBeUp ? TEXT("True") : TEXT("False"));

	return ShouldShieldBeUp;
}