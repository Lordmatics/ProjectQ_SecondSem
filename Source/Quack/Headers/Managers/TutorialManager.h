// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Headers/MyConstants.h"
#include "TutorialManager.generated.h"

UCLASS()
class QUACK_API ATutorialManager : public AActor
{
	GENERATED_BODY()
	
private:
	// THIS IS TEMPORARY - CHANGE TO SLIDING DOORS OR SOMETHING

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		class AAudioManager* AudioManager;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		TArray<AActor*> Walls;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		TArray<class AQuackAIPawn*> Worms;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		TArray<class APipe*> Pipes;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		uint32 bGravGunPickedUp : 1;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		uint32 bPoisonUsed : 1;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		uint32 bRockThrown : 1;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		uint32 bEnemiesDestroyed : 1;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		class ALockableDoor* GravDoor;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		class ALockableDoor* MinionRoomDoor;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		TArray<AActor*> SpawnLocations;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		TSubclassOf<class AQuackAIPawn> EnemyClass;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial Events")
		TArray<class AQuackAIPawn*> RoomEnemies;

	UPROPERTY(EditAnywhere, Category = "Tutorial Events")
		int NumberEnemiesRoom_01 = 4;

	UPROPERTY(EditAnywhere, Category = "Bodge")
		uint32 bDoOnce : 1;
private:
	bool SetWallOff(AActor* WhichWall);

	void SpawnRoomEnemies(int EnemiesInRoom);

	void ManageTheTutorial();
public:
	// Sets default values for this actor's properties
	ATutorialManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void WormDiedAt(int _ID);

	UFUNCTION()
	void RemoveAPipe();

	UFUNCTION()
		void RemoveEnemy(class AQuackAIPawn* _Enemy);

	FORCEINLINE void TrialGravGunPickUp() { bGravGunPickedUp = true; }
	FORCEINLINE void TrialPoisonUsed() { bPoisonUsed = true; }
	FORCEINLINE void TrialRockThrown() { bRockThrown = true; }
	FORCEINLINE void TrialEnemiesDestroyed() { bEnemiesDestroyed = true; }
	
};
