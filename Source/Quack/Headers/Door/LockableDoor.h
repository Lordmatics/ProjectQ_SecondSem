// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LockableDoor.generated.h"

UCLASS()
class QUACK_API ALockableDoor : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "DoorComponents")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "DoorComponents")
		UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleDefaultsOnly, Category = "DoorComponents")
		UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleDefaultsOnly, Category = "DoorComponents")
		UStaticMeshComponent* RightDoor;

	UPROPERTY(VisibleDefaultsOnly, Category = "DoorComponents")
		UBoxComponent* BoxEnter;

	UPROPERTY(VisibleDefaultsOnly, Category = "DoorComponents")
		UBoxComponent* BoxExit;

private:
	UPROPERTY(EditAnywhere, Category = "DoorVariables")
		uint32 bLockable : 1;

	UPROPERTY(EditAnywhere, Category = "DoorVariables")
		uint32 bLocked : 1;

	UPROPERTY(EditAnywhere, Category = "DoorVariables")
		uint32 bIsOpen : 1;

	UPROPERTY(EditAnywhere, Category = "DoorVariables")
		float OpenSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "DoorVariables")
		float OpenLength = 200.0f;

	UPROPERTY(VisibleAnywhere, Category = "DoorVariables")
		FVector LeftStartLocation;

	UPROPERTY(VisibleAnywhere, Category = "DoorVariables")
		FVector RightStartLocation;

	UPROPERTY(EditAnywhere, Category = "DoorVariables")
		UMaterialInstance* LockedMat;

	UPROPERTY(EditAnywhere, Category = "DoorVariables")
		UMaterialInstance* UnlockedMat;

public:	
	// Sets default values for this actor's properties
	ALockableDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void CloseAndLock();

	void OpenAndUnlock(bool _OpenToo);

private:

	UFUNCTION()
		void OnEnterStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtheComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEnterEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtheComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnExitStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtheComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void OpenDoor(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void CloseDoor(float DeltaTime);
	
};
