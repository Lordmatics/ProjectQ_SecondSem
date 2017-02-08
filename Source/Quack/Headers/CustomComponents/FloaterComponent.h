// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "FloaterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UFloaterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFloaterComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	// BitField of 1 for bool
	UPROPERTY(EditAnywhere, Category = "Custom Niall")
		uint8 bCanFloat : 1;

	// Time
	UPROPERTY(VisibleAnywhere, Category = "Custom Niall")
		float CurrentTime = 0.0f;

	// Speed
	UPROPERTY(EditAnywhere, Category = "Custom Niall")
		float Speed = 3.0f;

	// Height
	UPROPERTY(EditAnywhere, Category = "Custom Niall")
		float Height = 50.0f;

	void Float(float DeltaTime);
};
