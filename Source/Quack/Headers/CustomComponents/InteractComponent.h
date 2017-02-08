// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Interactable)
		uint32 bCanBePickedUp : 1;

	// 0 gun, 1 for needle
	UPROPERTY(EditAnywhere, Category = Interactable)
		int InteractID = 0;
public:	
	// Sets default values for this component's properties
	UInteractComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	FORCEINLINE void SetPickUpable(bool _State) { bCanBePickedUp = _State; }
	FORCEINLINE bool GetPickUpable() const { return bCanBePickedUp; }

	FORCEINLINE void SetID(int _ID) { InteractID = _ID; }
	FORCEINLINE int GetID() const { return InteractID; }
	
};
