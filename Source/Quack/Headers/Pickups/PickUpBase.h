// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

UCLASS()
class QUACK_API APickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
		USceneComponent* MyRoot;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
	//	UShapeComponent* MyTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
		UStaticMeshComponent* MyStaticMesh;
	
	UFUNCTION()
		virtual void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
};
