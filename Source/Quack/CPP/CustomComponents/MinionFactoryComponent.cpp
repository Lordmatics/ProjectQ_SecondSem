// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/MinionFactoryComponent.h"
#include "Headers/AIEnemies/QuackAIPawn.h"
#include "Headers/Misc/MinionSpawnPoint.h"

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

	//TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinionSpawnPoint::StaticClass(), SpawnPoints);
	// ...
	
}


// Called every frame
void UMinionFactoryComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

/*void UMinionFactoryComponent::SpawnMinionWaveA()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (SpawnPointsA.Num() == 0) return;
	if (EnemyFactoryA.Num() == 0) return;
	for (size_t i = 0; i < SpawnPointsA.Num(); i++)
	{
		int EnemyIndex = i > EnemyFactoryA.Num() - 1 ? 0 : FMath::RandRange(0, EnemyFactoryA.Num() - 1);
		if (SpawnPointsA[i] == nullptr) return;
		AQuackAIPawn* Minion = World->SpawnActor<AQuackAIPawn>(EnemyFactoryA[EnemyIndex], SpawnPointsA[i]->GetActorLocation(), FRotator::ZeroRotator);
		if (Minion != nullptr)
		{
			Minion->SetBossMinion();
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
		if (SpawnPointsB[i] == nullptr) return;
		AQuackAIPawn* Minion = World->SpawnActor<AQuackAIPawn>(EnemyFactoryB[EnemyIndex], SpawnPointsB[i]->GetActorLocation(), FRotator::ZeroRotator);
		if (Minion != nullptr)
		{
			Minion->SetBossMinion();
			Minion->OnEnemyDestroyed.AddDynamic(this, &UMinionFactoryComponent::RemoveFromListB);
			MinionArrayB.Add(Minion);
		}
	}
}*/

void UMinionFactoryComponent::SpawnMinionWave(int MinionsToSpawn)
{
	UWorld* const World = GetWorld();
	if (World == nullptr || SpawnPoints.Num() == 0 || EnemyFactory.Num() == 0) return;
	if (MinionsToSpawn > SpawnPoints.Num())
	{
		MinionsToSpawn = SpawnPoints.Num();
	}
	for (int32 i = SpawnPoints.Num() - 1; i > 0; i--) {
		int32 j = FMath::FloorToInt(FMath::FRand() * (i + 1));
		AActor* Temp = SpawnPoints[i];
		SpawnPoints[i] = SpawnPoints[j];
		SpawnPoints[j] = Temp;
	}
	for (size_t i = 0; i < MinionsToSpawn; i++)
	{
		int EnemyIndex = i > EnemyFactory.Num() - 1 ? 0 : FMath::RandRange(0, EnemyFactory.Num() - 1);
		if (SpawnPoints[i] == nullptr) return;
		AQuackAIPawn* Minion = World->SpawnActor<AQuackAIPawn>(EnemyFactory[EnemyIndex], SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
		if (Minion != nullptr)
		{
			Minion->SetBossMinion();
			Minion->OnEnemyDestroyed.AddDynamic(this, &UMinionFactoryComponent::RemoveFromList);
			MinionArray.Add(Minion);
		}
	}
}

/*void UMinionFactoryComponent::RemoveFromListA(AQuackAIPawn* _Enemy)
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
}*/

void UMinionFactoryComponent::RemoveFromList(AQuackAIPawn* _Enemy)
{
	MinionArray.Remove(_Enemy);
}

bool UMinionFactoryComponent::AreMinionsAlive()
{
	/*int A = MinionArrayA.Num();
	//UE_LOG(LogTemp, Warning, TEXT("Array A Length: %d"), A);
	int B = MinionArrayB.Num();
	*/
	int C = MinionArray.Num();
	//UE_LOG(LogTemp, Warning, TEXT("Array B Length: %d"), B);
	bool ShouldShieldBeUp = C > 0 ? true : false;
	//UE_LOG(LogTemp, Warning, TEXT("Array B Length: %s"), ShouldShieldBeUp ? TEXT("True") : TEXT("False"));

	return ShouldShieldBeUp;
}