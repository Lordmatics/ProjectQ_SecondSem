// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Headers/MyConstants.h"
#include "AudioZoneBox.generated.h"

UCLASS()
class QUACK_API AAudioZoneBox : public AActor
{
	GENERATED_BODY()
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Root")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Boxes")
		UBoxComponent* EnemyRoomTrigger;

	UFUNCTION()
		void MinionRoomEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Volume")
		float FadeInDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Volume")
		float NewTrackVolume = 0.95f;

	UPROPERTY(EditAnywhere, Category = "Timers")
		uint32 bIsBossTrigger : 1;

	UPROPERTY(EditAnywhere, Category = "Timers")
		float BossIntroLength = 3.0f;

	UPROPERTY(VisibleAnywhere, Category = "AudioState")
		class AAudioManager* AudioManager;

	UPROPERTY(EditAnywhere, Category = "AudioState")
		AudioStates StateToTransitionTo;

	UPROPERTY(EditAnywhere, Category = "Timers")
		uint32 bCanBeLocked : 1;

	UPROPERTY(EditAnywhere, Category = "Timers")
		uint32 bIsCombatTrigger : 1;

	UFUNCTION()
		void PlayBossLoop(float Fade, float Volu);
public:	
	// Sets default values for this actor's properties
	AAudioZoneBox();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
