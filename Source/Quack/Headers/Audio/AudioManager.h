// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Headers/MyConstants.h"
#include "AudioManager.generated.h"

//UENUM(BlueprintType)        //"BlueprintType" is essential specifier
//							//256 entries max
//	enum class AudioStates : uint8
//{
//	E_BossIntro     UMETA(DisplayName = "BossIntro"),
//	E_BossLoop         UMETA(DisplayName = "BossLoop"),
//	E_CombatLoop         UMETA(DisplayName = "Combat"),
//	E_EnemiesNearLoop         UMETA(DisplayName = "EnemiesNear"),
//	E_ExplorationLoop         UMETA(DisplayName = "Exploring"),
//	E_Victory        UMETA(DisplayName = "Victory"),
//};

UCLASS()
class QUACK_API AAudioManager : public AActor
{
	GENERATED_BODY()
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Root")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Audio")
		UAudioComponent* AudioComponentA;

	UPROPERTY(VisibleDefaultsOnly, Category = "Audio")
		UAudioComponent* AudioComponentB;

	UPROPERTY(EditAnywhere, Category = "Audio")
		AudioStates PreviousAudioState;

	UPROPERTY(EditAnywhere, Category = "Audio")
		AudioStates CurrentAudioState;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* BossIntro;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* BossLoop;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* CombatLoop;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* EnemiesAreNearLoop;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* ExplorationLoop;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* BossVictory;

	UPROPERTY(EditAnywhere, Category = "Audio")
		uint32 bAudioBIsPlaying : 1;

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Boxes")
		UBoxComponent* EnemyRoomTrigger;

	//UFUNCTION()
		//void MinionRoomEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	void HandleFadesToNewTrack(USoundWave* _NewTrack, float Fade, float Volu);
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//void FadeTo(AudioStates _State);
	void ChangeStateTo(AudioStates _NewState, float Fade, float Volu);

	
};
