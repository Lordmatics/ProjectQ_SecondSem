// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "QuackAIPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDestroyed, AQuackAIPawn*, _Minion);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDestroyedRoom, AQuackAIPawn*, _Minion);

UCLASS()
class QUACK_API AQuackAIPawn : public ACharacter
{
	GENERATED_BODY()

protected:

	//UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//	USceneComponent* SceneComponent;

	//UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//	class UCharacterMovementComponent* MovementComp;

	//UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		//USkeletalMeshComponent* SkeletalComp;

	UPROPERTY(EditAnywhere, Category = "Tutorial Manager")
		class ATutorialManager* TutorialManager;

	UPROPERTY(EditAnywhere, Category = "Tutorial Manager")
		int WormID = 0;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollision;

	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MaxHealth;
	float CurrentHealth;
	UPROPERTY(Category = "Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Damage;

public:
	// Sets default values for this pawn's properties
	AQuackAIPawn();

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Attack();

	virtual void TakeDamages(float DamageIn);

	virtual void Die();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	UPROPERTY(EditAnywhere, Category = "Delegate")
		FOnEnemyDestroyed OnEnemyDestroyed;

	UPROPERTY(EditAnywhere, Category = "Delegate")
		FOnEnemyDestroyedRoom OnEnemyDestroyedRoom;
};
