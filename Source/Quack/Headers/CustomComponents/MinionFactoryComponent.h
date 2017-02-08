// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MinionFactoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UMinionFactoryComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<TSubclassOf<class AQuackAIPawn>> EnemyFactoryA;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<TSubclassOf<class AQuackAIPawn>> EnemyFactoryB;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<AActor*> SpawnPointsA;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<AActor*> SpawnPointsB;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<class AQuackAIPawn*> MinionArrayA;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<class AQuackAIPawn*> MinionArrayB;

	UFUNCTION()
		void RemoveFromListA(class AQuackAIPawn* _Enemy);

	UFUNCTION()
		void RemoveFromListB(class AQuackAIPawn* _Enemy);
public:	
	// Sets default values for this component's properties
	UMinionFactoryComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void SpawnMinionWaveA();

	void SpawnMinionWaveB();
	
	bool AreMinionsAlive();
};
