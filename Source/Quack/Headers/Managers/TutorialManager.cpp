// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Managers/TutorialManager.h"
#include "Headers/AIEnemies/QuackAIPawn.h"
#include "Headers/Misc/Pipe.h"
#include "Headers/Door/LockableDoor.h"
#include "Headers/Audio/AudioManager.h"
#include "EngineUtils.h"

// Sets default values
ATutorialManager::ATutorialManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AAudioManager> ActorItr(World); ActorItr; ++ActorItr)
		{
			AudioManager = *ActorItr;
		}
	}

}

// Called when the game starts or when spawned
void ATutorialManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnRoomEnemies(NumberEnemiesRoom_01);
}

// Called every frame
void ATutorialManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	ManageTheTutorial();
}

void ATutorialManager::SpawnRoomEnemies(int EnemiesInRoom)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (SpawnLocations.Num() == 0) return;
	if (EnemiesInRoom >= SpawnLocations.Num() + 1)
		EnemiesInRoom = 1;
	for (size_t i = 0; i < EnemiesInRoom; i++)
	{
		if (EnemyClass != nullptr && SpawnLocations[i] != nullptr)
		{
			AQuackAIPawn* Enemy = World->SpawnActor<AQuackAIPawn>(EnemyClass, SpawnLocations[i]->GetActorLocation(), FRotator::ZeroRotator);
			if (Enemy != nullptr)
			{
				RoomEnemies.Add(Enemy);
				Enemy->OnEnemyDestroyedRoom.AddDynamic(this, &ATutorialManager::RemoveEnemy);
			}
		}

	}
}

void ATutorialManager::RemoveEnemy(AQuackAIPawn* _Enemy)
{
	if (RoomEnemies.Num() == 0) return;
		RoomEnemies.Remove(_Enemy);
}

void ATutorialManager::RemoveAPipe()
{
	if (Pipes.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Pipe Removed"));
		Pipes.RemoveAt(0);

	}
}

void ATutorialManager::ManageTheTutorial()
{
	if (Walls.Num() == 0) return;
	if (bGravGunPickedUp)
	{
		if (Walls[0] != nullptr)
		{
			if (SetWallOff(Walls[0]))
			{
				Walls[0] = nullptr;
			}
			if(GravDoor != nullptr)
				GravDoor->OpenAndUnlock(false);
		}
	}
	if (bPoisonUsed)
	{
		if (Walls[1] != nullptr)
		{
			if (Pipes.Num() <= 0.0f)
			{
				// TEMPORARY
				//AudioManager->ChangeStateTo(AudioStates::E_EnemiesNearLoop, 1.5f, 1.0f);
				if (SetWallOff(Walls[1]))
				{
					Walls[1] = nullptr;
				}
			}
		}
	}
	if (bRockThrown)
	{
		if (Walls[2] != nullptr)
		{
			if (SetWallOff(Walls[2]))
			{
				Walls[2] = nullptr;
			}
		}
	}
	if (RoomEnemies.Num() == 0)
	{
		//bEnemiesDestroyed = true;
		if (MinionRoomDoor != nullptr)
		{
			MinionRoomDoor->OpenAndUnlock(false);
			if (!bDoOnce)
			{
				AudioManager->ChangeStateTo(AudioStates::E_ExplorationLoop, 1.5f, 1.0f);
				bDoOnce = true;
			}
		}
	}
	if (bEnemiesDestroyed)
	{
		if (Walls[3] != nullptr)
		{
			if (SetWallOff(Walls[3]))
			{
				Walls[3] = nullptr;
			}
		}
	}
	//if (Worms.Num() >= 4)
	//{
	//	if (Worms[0] == nullptr && Worms[1] == nullptr && Worms[2] == nullptr && Worms[3] == nullptr)
	//	{
	//		bEnemiesDestroyed = true;
	//	}
	//}
}

void ATutorialManager::WormDiedAt(int _ID)
{
	if (Worms.Num() == 0) return;
	Worms[_ID] = nullptr;
}

bool ATutorialManager::SetWallOff(AActor* WhichWall)
{
	// More efficient to stop renderring than destroy at runtime
	// STILL TEMPORARY
	if (WhichWall != nullptr)
	{
//		WhichWall->SetHiddenInGame(true, true);
		WhichWall->SetActorHiddenInGame(true);
		WhichWall->SetActorEnableCollision(false);
		return true;
//		WhichWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		return false;
	}
}