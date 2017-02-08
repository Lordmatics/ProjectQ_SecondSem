// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HealthStation.generated.h"

UCLASS()
class QUACK_API AHealthStation : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		USceneComponent* MyRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MyStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class UShapeComponent* MyTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float HealingAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float HealingCapacity = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float Volume = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		uint32 bIsInZone : 1;

	UPROPERTY(VisibleAnywhere, Category = "Custom")
		class AQuackCharacter* MyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		USoundBase* JuggernogSound;

private:
	void InitialiseCharacterReference();
	void PlaySound();
	void ToggleHighlight(bool State);
public:	
	// Sets default values for this actor's properties
	AHealthStation();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

public:

	FORCEINLINE bool GetIsInZone()const { return bIsInZone; }
	FORCEINLINE void SetIsInZone(bool State) { bIsInZone = State; }
	
};
