// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

UCLASS()
class QUACK_API ABaseGun : public AActor
{
	GENERATED_BODY()
private:

public:	
	// Sets default values for this actor's properties
	ABaseGun();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Set as main weapon, and make it visible */
	virtual void WieldAndActivate();

	/** Set as secondary weapon, and make it invisible */
	virtual void SheathAndDeactivate();

	// Makes Guns face the right way on spawn
	virtual void ResetOrientation();

	virtual USkeletalMeshComponent* GetGunMesh() const;

	FORCEINLINE float GetGunDamage() const { return GunDamage; }
	
	FORCEINLINE bool GetIsActive() const { return bIsActive; }

	FORCEINLINE bool GetIsReloading() const { return bIsReloading; }

	FORCEINLINE bool IsFullyAutomatic() const { return bIsFullyAutomatic; }

	FORCEINLINE bool InUseUnableToSwap() const { return bIsInUseUnableToSwap; }
	/** set the weapon's owning pawn */
	void SetOwningPawn(class AQuackCharacter* MyPlayerRef);

	FName SocketName = FName(TEXT("MuzzleFlashSocket"));

	virtual void Shoot();

	virtual void StopMuzzleFlash();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		float GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		float GetMaxAmmo() const;

	float Reload(USkeletalMeshComponent* SpecificGun);

	void IncreaseAmmo(float Amount);

	void DisableGun();

	void EnableGun();

protected:

	UPROPERTY(EditAnywhere, Category = "C++ Gun")
		uint32 bIsFullyAutomatic : 1;

	UPROPERTY(EditAnywhere, Category = "C++ Gun")
		uint32 bIsInUseUnableToSwap : 1;
	

	void DecreaseAmmo(float Amount, USkeletalMeshComponent* SpecificGun);

	void CheckForMaxClip();

	virtual void AttachMeshToPawn();


	void PlayFeedbackShake();

	void PlayFeedbackAudio(USkeletalMeshComponent* SpecificGun);
protected:

	TArray<TWeakObjectPtr<AActor>> IgnoredActors;

	UFUNCTION()
		void FinishReload();

	/** pawn owner */
	UPROPERTY()
		class AQuackCharacter* MyPawn;

	UPROPERTY(EditAnywhere, Category = "C++ Gun Variables")
		uint32 bIsReloading : 1;

	/** name of bone/socket for muzzle in weapon mesh */
	UPROPERTY(EditAnywhere, Category = "C++ Gun Variables")
		FName MuzzleAttachPoint;

	UPROPERTY(VisibleAnywhere, Category = "C++ Gun Variables")
		uint32 bIsActive : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		float GunShootVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		int MaxClipSize = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		float Ammo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		float ReservedAmmo = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		float FireRate = 0.15f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ Gun Variables")
		float FireCooldown = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ Gun Variables")
		float InitialFireCooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		float GunDamage = 15.0f;

	bool IsDisabled;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		FVector GunOffset = FVector(100.0f, 30.0f, 10.0f);

	/** Projectile class to spawn */
	//UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	//	TArray<TSubclassOf<class AQuackProjectile>> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		TSubclassOf<UCameraShake> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Gun Variables")
		UForceFeedbackEffect* Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* FireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystemComponent* DisabledEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* FireTrailEffect;

	UPROPERTY()
		UParticleSystemComponent* MuzzleFlashParticleSystem;

	float RunningTime;

	FTimerHandle ContinuousShooting;

	FTimerHandle FireRateDelay;



	
};
