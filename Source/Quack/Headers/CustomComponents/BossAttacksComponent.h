// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Structs_Enums.h"
#include "BossAttacksComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UBossAttacksComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	// My Variables - Bile Spit
	FTimerHandle BileTimer;

	/** Bool to determine if boss is bile shooting*/
	UPROPERTY(EditAnywhere, Category = "C++ Boss Variables")
		uint32 bIsBileSpitting : 1;

	/** Float to determine how frequently boss shoots*/
	UPROPERTY(EditAnywhere, Category = "C++ Boss Variables")
		float BileFireRate = 0.5f;

	/** Int to determine how far the boss should shoot*/
	UPROPERTY(EditAnywhere, Category = "C++ Boss Variables")
		int BileShotsFired = 1;

	/** Array of projectiles that the boss could potentially use*/
	UPROPERTY(EditAnywhere, Category = "C++ Boss Variables")
		TArray <TSubclassOf<class AQuackProjectile>> BossProjectilesArray;

private:
	// My Variables - Tail Shot
	FTimerHandle TailTimer;

	/** Bool to determine if boss is tail shooting*/
	UPROPERTY(EditAnywhere, Category = "C++ Boss Variables")
		uint32 bIsTailShooting : 1;

	/** Float to determine how frequently boss shoots*/
	UPROPERTY(EditAnywhere, Category = "C++ Boss Variables")
		float TailFireRate = 5.0f;
public:	
	// Sets default values for this component's properties
	UBossAttacksComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

public:
	// My Functions - Bile Spit

	/** Function to initiate the bile spitting*/
	UFUNCTION()
		void StartBileSpitting(class UArrowComponent* MouthArrow, class UAnimInstance* BossAnimInstance, class UAnimationComponent* BossAnimComponent, float OverridenFireRate = 1.0f);

	/** Function that plays the animation for the attack, with an appropriate delay, for the attack to match the animation*/
	UFUNCTION()
		void BeginShootTheBile(UArrowComponent* MouthArrow, UAnimInstance* BossAnimInstance, UAnimationComponent* BossAnimComponent);

	/** Function that actually fires the projectile*/
	UFUNCTION()
		void ShootTheBile(class UArrowComponent* MouthArrow, class UAnimInstance* BossAnimInstance, class UAnimationComponent* BossAnimComponent);

	/** Function to end the bile spitting*/
	void StopBileSpitting();

	/** Utility function to determine is bile spitting in motion*/
	FORCEINLINE bool GetIsBileSpitting() const { return bIsBileSpitting; }

	/** Utility function to get the current fire rate of the boss*/
	FORCEINLINE float GetBileFireRate() const { return BileFireRate; }

public:
	// My Functions - Tail Shot

	/** Function to initiate the tail shooting*/
	UFUNCTION()
		void StartTailShooting(class UArrowComponent* TailArrow, class UAnimInstance* BossAnimInstance, class UAnimationComponent* BossAnimComponent, float OverridenFireRate = 1.0f);

	/** Function that plays the animation for the attack, with an appropriate delay, for the attack to match the animation*/
	UFUNCTION()
		void BeginTailShoot(UArrowComponent* TailArrow, UAnimInstance* BossAnimInstance, UAnimationComponent* BossAnimComponent);

	/** Function that actually fires the projectile*/
	UFUNCTION()
		void TailShoot(class UArrowComponent* TailArrow, class UAnimInstance* BossAnimInstance, class UAnimationComponent* BossAnimComponent);

	/** Function to end the tail shooting*/
	void StopTailShoot();

	/** Utility function to determine is tail shooting in motion*/
	FORCEINLINE bool GetIsTailShooting() const { return bIsTailShooting; }

	/** Utility function to get the current fire rate of the boss*/
	FORCEINLINE float GetTailFireRate() const { return TailFireRate; }
public:
	// My Variables

	
};
