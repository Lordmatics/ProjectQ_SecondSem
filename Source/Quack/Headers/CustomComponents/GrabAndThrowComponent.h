// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "GrabAndThrowComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UGrabAndThrowComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		bool bObjectHeld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float MinGrabLength = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		bool bPhysicsHandleActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float RayLength = 5000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float VectorLength = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float ThrowForce = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float ObjectWeightLimit = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float MaxRockWeight = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		FVector HandleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		TArray<TEnumAsByte<EObjectTypeQuery>> PhysicsObjType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
		UPrimitiveComponent* HitComponent;
private:
	void ReleaseRock();

public:
	UPROPERTY(VisibleAnywhere, Category = "Custom")
		class ARock* CurrentRock;
public:
	// Sets default values for this component's properties
	UGrabAndThrowComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Throw(class UPhysicsHandleComponent* PhysicsHandle, UCameraComponent* FirstPersonCameraComponent, bool MD);
	void Grab(UObject* WorldContextObject, UPhysicsHandleComponent* PH, UCameraComponent* FirstPersonCameraComponent);
	void Release(class UPhysicsHandleComponent* PhysicsHandle);
	void TraceHandleLocation(class UPhysicsHandleComponent* PhysicsHandle, UCameraComponent* FirstPersonCameraComponent);

	void MeleeRockRelease();

	FORCEINLINE bool GetIsObjectHeld() const { return bObjectHeld; }

};
