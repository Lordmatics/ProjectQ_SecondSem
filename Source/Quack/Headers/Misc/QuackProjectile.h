// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "QuackProjectile.generated.h"

UCLASS(config=Game)
class AQuackProjectile : public AActor
{
	GENERATED_BODY()

		/** Sphere collision component */
private:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USceneComponent* MyRoot;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	void AdjustProjectileMovement(float _InitSpd, float _MaxVel);



public:
	void AdjustProjectileMovementScale(float _RandomVelScale);

	AQuackProjectile();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float GravityForce = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float InitialVel = 15000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float MaxVel = 15000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		bool bCanBounce = true;
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SomethingHit(AActor* OtherActor);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		float ProjectileDamage = 0.1f;
};

