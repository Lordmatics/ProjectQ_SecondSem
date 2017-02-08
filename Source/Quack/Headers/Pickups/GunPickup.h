// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GunPickup.generated.h"

UCLASS()
class QUACK_API AGunPickup : public AActor
{
	GENERATED_BODY()
	
private:

	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex);

private:
	UPROPERTY(EditAnywhere, Category = "Custom Components")
		USceneComponent* MyRoot;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Components")
		USkeletalMeshComponent* MySkeletalMesh;
	
	UPROPERTY(EditAnywhere, Category = "Custom Components")
		UBoxComponent* MyTrigger;

	UPROPERTY(EditAnywhere, Category = "Custom Components")
		class UInteractComponent* InteractClass;
public:	
	// Sets default values for this actor's properties
	AGunPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
