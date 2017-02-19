// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "RaycastComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API URaycastComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Raycast")
		float Raylength = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Raycast")
		UMaterialInterface* BulletHoleDecal;

	UPROPERTY(EditAnywhere, Category = "Raycast")
		FVector DecalSize = FVector(3.5f, 7.0f, 7.0f);
	
	UPROPERTY(EditAnywhere, Category = "Raycast")
		float BulletDecalLifetime = 20.0f;

	void CreateBulletHole(FHitResult Object);
public:	
	// Sets default values for this component's properties
	URaycastComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	FHitResult Raycast(UCameraComponent* FirstPersonCamera, const TArray<TWeakObjectPtr<AActor>>& IgnoredActors = TArray<TWeakObjectPtr<AActor>>());
	
	FHitResult RaycastLaser(UParticleSystemComponent* FirstPersonCamera, const TArray<TWeakObjectPtr<AActor>>& IgnoredActors = TArray<TWeakObjectPtr<AActor>>());

	FHitResult RaycastBossLaser(UParticleSystemComponent* FirstPersonCamera, const FVector& EndLocation, const TArray<TWeakObjectPtr<AActor>>& IgnoredActors = TArray<TWeakObjectPtr<AActor>>());

	FORCEINLINE float GetRayLength() const { return Raylength; }

	FORCEINLINE UMaterialInterface* GetBulletHoleDecal() const { return BulletHoleDecal; }
	
};
