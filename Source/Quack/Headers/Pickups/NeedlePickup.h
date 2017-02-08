// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NeedlePickup.generated.h"

UCLASS()
class QUACK_API ANeedlePickup : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Custom Components")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Components")
		UStaticMeshComponent* MyStaticMesh;

	UPROPERTY(EditAnywhere, Category = "Custom Components")
		UBoxComponent* MyTrigger;

	UPROPERTY(EditAnywhere, Category = "Custom Components")
		class UInteractComponent* InteractClass;
public:	
	// Sets default values for this actor's properties
	ANeedlePickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
