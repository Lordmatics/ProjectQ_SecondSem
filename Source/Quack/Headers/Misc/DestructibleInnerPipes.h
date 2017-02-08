// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DestructibleInnerPipes.generated.h"

UCLASS()
class QUACK_API ADestructibleInnerPipes : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
		UBoxComponent* MyTrigger;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
		UStaticMeshComponent* MyStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
		class UDestructibleComponent* MyDestructibleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		class URadialForceComponent* RadialForce;
private:

	UFUNCTION()
		void OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection);

	UFUNCTION()
		void DestroyThis();

	UFUNCTION()
		void DisableCollision();
public:	
	// Sets default values for this actor's properties
	ADestructibleInnerPipes();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Fracture();
	
};
