// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BileSplashEffect.generated.h"

UCLASS()
class QUACK_API ABileSplashEffect : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Bile")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Bile")
		UStaticMeshComponent* TEMPBile;

	UPROPERTY(EditAnywhere, Category = "Bile")
		float DOTFrequency = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Bile")
		float BileDamageOverTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Bile")
		class AQuackCharacter* CharRef;

	FTimerHandle BileTimerDOT;

	virtual void StartDOT();

	void ClearDOT();

	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void DOTDamage();
public:	
	// Sets default values for this actor's properties
	ABileSplashEffect();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
