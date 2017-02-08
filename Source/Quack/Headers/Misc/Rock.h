// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rock.generated.h"

UCLASS()
class QUACK_API ARock : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Custom Niall")
		uint32 bIsGroundRock : 1;

	UPROPERTY(EditAnywhere, Category = "Custom Niall")
		class ATutorialManager* TutorialManager;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MyStaticMesh;

	UFUNCTION()
		virtual void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		float RockLifeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* RockParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		class USoundBase* RockSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		class UMaterialInstance* HeavyMaterial;

	FTimerHandle SelfDestruct;

	UPROPERTY(VisibleAnywhere, Category = "Grab Owner")
		class UGrabAndThrowComponent* GrabOwner;

	UFUNCTION()
		void DestroyMe();

	void ReleaseFromGrabber();

	void SpawnHitEffect(FVector Location);
public:	
	// Sets default values for this actor's properties
	ARock();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void InitiateSelfDestruct();
	void ToggleGravity(bool Toggle);

	void SetGrabOwner(class UGrabAndThrowComponent* Grabber);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Niall")
		bool bIsHeld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
		float RockSize = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Niall")
		float RockDamage = 25.0f;

	void SetHeavyMesh();


};
