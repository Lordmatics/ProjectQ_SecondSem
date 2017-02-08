// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pipe.generated.h"

//UENUM(BlueprintType)
//enum class EStencilColour : uint8
//{
//	SC_GREEN = 250 UMETA(DisplayName = "Green"),
//	SC_BLUE = 251 UMETA(DisplayName = "Blue"),
//	SC_RED = 252 UMETA(DisplayName = "Red"),
//	SC_WHITE = 253 UMETA(DisplayName = "White")
//};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPipeEmpty);

UCLASS()
class QUACK_API APipe : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
	USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
	UShapeComponent* MyTrigger;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
	UStaticMeshComponent* MyStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
		UStaticMeshComponent* DrainMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
		UStaticMeshComponent* PipeOuterBase;

	UPROPERTY(EditAnywhere, Category = "Custom Niall")
		UMaterialInstance* PoisonnedPipeMaterial;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
		uint32 bFinishedDraining : 1;
	


	//UPROPERTY(EditAnywhere, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
	//	EStencilColour Colour = EStencilColour::SC_BLUE;// = EStencilColour::SC_BLUE;

	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex);

	UPROPERTY(EditAnywhere, Category = "Pipe Movement")
		float MinPipeHeight = -1600.0f;

	UPROPERTY(EditAnywhere, Category = "Pipe Movement")
		float MaxPipeHeight = -1000.0f;

	UPROPERTY(EditAnywhere, Category = "Pipe Movement")
		float DrainSpeed = 65.0f;

	UPROPERTY(EditAnywhere, Category = "Pipe Movement")
		uint32 bDescend : 1;

	UPROPERTY(EditAnywhere, Category = "Pipe Movement")
		uint32 bDrained : 1;

	UPROPERTY(EditAnywhere, Category = "Pipe Movement")
		uint32 bDestroyed : 1;

	UPROPERTY(EditAnywhere, Category = "Pipe Movement")
		FVector SpawnLocation;

	void BeginDescend(float DeltaTime);

	void BeginAscend(float DeltaTime);
public:
	// Sets default values for this actor's properties
	APipe();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void ChangeMeshColour();

	void SimulateDestroy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
		bool bTargettedByBoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall")
		bool bPoisonedPipe;

	UPROPERTY(VisibleDefaultsOnly, Category = "Custom Niall")
		FOnPipeEmpty OnPipeFinishedDraining;

	void ToggleHighlight(bool Glow);

	void SetDescend(bool _State);

	UPROPERTY(EditAnywhere, Category = "Pipe")
		uint32 bNotABossPipe : 1;

	UPROPERTY(EditAnywhere, Category = "Pipe")
		UParticleSystemComponent* LeakParticleSys;

	FORCEINLINE bool IsFinishedDraining() const { return bFinishedDraining; }
protected:

};
