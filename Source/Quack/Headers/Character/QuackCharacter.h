// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "QuackCharacter.generated.h"

class UInputComponent;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	//	bool bIsUsingGravityGun;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	//	bool bHasGravityGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		bool bMouseDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		bool bMouseUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float FireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float FireCooldown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float InitialFireCooldown;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		FVector GunOffset;

	float RunningTime;

	FTimerHandle ContinuousShooting;

	FTimerHandle FireRateDelay;
	/** defaults */
	FWeaponData()
	{
		//bIsUsingGravityGun = false;
		//bHasGravityGun = false;
		bMouseDown = false;
		bMouseUp = true;
		FireRate = 0.15f;
		FireCooldown = 0.0f;
		InitialFireCooldown = 0.0f;
		RunningTime = 0.0f;
	}
};

USTRUCT(BlueprintType)
struct FPoisonData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		bool bHasNeedle;

	// Used in boss class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		bool bIsPoisoning;

	// Used in pipe class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		bool bCanPoisonPipe;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float PoisonCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		int PoisonVialsCount;

	FTimerHandle PoisonUsageDelay;

	/** defaults */
	FPoisonData()
	{
		bHasNeedle = false;
		bIsPoisoning = false;
		bCanPoisonPipe = false;
		PoisonCD = 2.5f;
		PoisonVialsCount = 4;
	}
};

USTRUCT(BlueprintType)
struct FMovementData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float InitialWalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		uint32 bIsSprinting : 1;

	FMovementData()
	{
		SprintSpeed = 1500.0f;
		InitialWalkSpeed = 1000.0f;
		bIsSprinting = false;
	}
};

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float RayLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class APipe* CurrentPipe;

	UPROPERTY(VisibleAnywhere, Category = "Custom")
		class AQuackBoss* Boss;

	FPlayerData()
	{
		Health = 30.0f;
		MaxHealth = 30.0f;
		RayLength = 5000.0f;
	}
};

UCLASS(config=Game)
class AQuackCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcess", meta = (AllowPrivateAccess = "true"))
		class UPostProcessComponent* PostProcessComponent;

	UPROPERTY(VisibleAnywhere, Category = "Guns")
		int PreviousEquipIndex = -1;

	UPROPERTY(VisibleAnywhere, Category = "Guns")
		int CurrentEquipIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Guns")
		TArray<TSubclassOf<class ABaseGun>> GunInventoryClasses;

	UPROPERTY(EditAnywhere, Category = "Guns")
		class AAssaultRifle* AssaultRifleRef;

	UPROPERTY(EditAnywhere, Category = "Guns")
		class APlasmaRifle* PlasmaRifleRef;

	UPROPERTY(EditAnywhere, Category = "Guns")
		class ABaseGun* CurrentEquippedGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns", meta = (AllowPrivateAccess = "true"))
		TArray<class ABaseGun*> GunInventory;

	void SpawnGunInventory();

	void HandleChangeToNewGun();

	UPROPERTY(EditAnywhere, Category = "Volume")
		float GunShootVolume = 0.5f;
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY()
		class UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(VisibleAnywhere, Category = "Interact")
		AActor* InteractableActor;

	UPROPERTY(VisibleAnywhere, Category = "Interact")
		int InteractID = -1;

	UPROPERTY(EditAnywhere, Category = "Tutorials")
		class ATutorialManager* TutorialManager;

	UFUNCTION()
		void Interact();

	UFUNCTION()
		void SwapGun();

	UFUNCTION()
		void OnSprintBegin();
	
	UFUNCTION()
		void OnSprintEnd();

	void Sprint();

	UPROPERTY()
		UPrimitiveComponent* HitComponent;

	UFUNCTION()
		void MouseDown();

	UFUNCTION()
		void MouseUp();

	void SemiAutomaticShooting(float DeltaTime);

	void PoisonCooldown();

	UFUNCTION()
		void UsePoison();

	UFUNCTION()
		void Reload();

	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

public:
	AQuackCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	bool bEnteredBossRoom;

	/** get weapon attach point */
	FName GetWeaponAttachPoint() const;

	USkeletalMeshComponent* GetSpecifcPawnMesh() const;

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	// Adds ammo to current gun
	// Note: Change ammo collection depending on type of gun
	void IncreaseAmmo(int Amount);

	void IncreaseHealth(float Amount);
	void IncreaseHealth(float Amount, float DeltaTime);

	void DecreaseHealth(float Amount);

	FORCEINLINE float GetCurrentHealth() const { return PlayerConfig.Health; }

	FORCEINLINE float GetMaxHealth() const { return PlayerConfig.MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
	int GetCurrentEquippedGunIndex() const;

	UPROPERTY()
		class UGrabAndThrowComponent* GrabberClass;

	// Used in boss class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BossHP;

	/** poison data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		FPoisonData PoisonConfig = FPoisonData();

	UFUNCTION(BlueprintCallable, Category = "Sprint")
		bool IsSprinting() const;

protected:

	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Niall", meta = (AllowPrivateAccess = "true"))
		FName WeaponAttachPoint = FName("GripPoint");

	/** weapon data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		FWeaponData WeaponConfig = FWeaponData();

	/** movement data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		FMovementData MovementConfig = FMovementData();

	/** movement data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		FPlayerData PlayerConfig = FPlayerData();

	void Raycast();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void PostInitializeComponents() override;

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

