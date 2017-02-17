// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Headers/Quack.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/Misc/QuackProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Headers/Misc/Pipe.h"
#include "Headers/Boss/QuackBoss.h"
#include "Headers/AIEnemies/QuackAIPawn.h"
#include "Headers/Misc/Rock.h"
#include "Headers/CustomComponents/GrabAndThrowComponent.h"
#include "Headers/Stations/HealthStation.h"
#include "Headers/CustomComponents/InteractComponent.h"
#include "Headers/Managers/TutorialManager.h"
#include "EngineUtils.h"
#include "Headers/Character/Guns/BaseGun.h"
#include "Headers/Character/Guns/AssaultRifle.h"
#include "Headers/Character/Guns/PlasmaRifle.h"
#include "Classes/Components/PostProcessComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AQuackCharacter

// AWAKE
AQuackCharacter::AQuackCharacter()
{

	

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(RootComponent);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Default offset from the character location for projectiles to spawn
	WeaponConfig.GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AQuackBoss> ActorItr(World); ActorItr; ++ActorItr)
		{
			PlayerConfig.Boss = *ActorItr;
		}
	}
}

void AQuackCharacter::SpawnGunInventory()
{
	UWorld* const World = GetWorld();
	// Early exits - should never want this to occur
	if (World == nullptr) return;
	if (GunInventoryClasses.Num() == 0) return;
	if (GunInventoryClasses[0] == nullptr || GunInventoryClasses[1] == nullptr) return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AAssaultRifle* AR = World->SpawnActor<AAssaultRifle>(GunInventoryClasses[0], SpawnInfo);
	if (AR != nullptr)
	{
		AssaultRifleRef = AR;
		AssaultRifleRef->SetOwningPawn(this);
		AssaultRifleRef->WieldAndActivate();
		CurrentEquippedGun = AssaultRifleRef;
		// Required for Raycast
		if(FirstPersonCameraComponent != nullptr)
			AssaultRifleRef->SetOwningCamera(FirstPersonCameraComponent);
		GunInventory.Add(AssaultRifleRef);
	}
	APlasmaRifle* PR = World->SpawnActor<APlasmaRifle>(GunInventoryClasses[1], SpawnInfo);
	if (PR != nullptr)
	{
		PlasmaRifleRef = PR;
		PlasmaRifleRef->SetOwningPawn(this);
		PlasmaRifleRef->SheathAndDeactivate();
		GunInventory.Add(PlasmaRifleRef);
	}
}

void AQuackCharacter::HandleChangeToNewGun()
{
	if (CurrentEquippedGun != nullptr)
	{
		// Alternatively, can cancel reload and force swap
		// Discuss what we want
		if (CurrentEquippedGun->GetIsReloading())
		{
			return;
		}
	}
	// Thinking about it, this would need to change if we have more than two guns
	//for (ABaseGun* Gun : GunInventory)
	//{
	//	if (Gun == nullptr) continue;
	//	if (Gun->GetIsActive())
	//	{
	//		Gun->SheathAndDeactivate();
	//	}
	//	else
	//	{
	//		Gun->WieldAndActivate();
	//		CurrentEquippedGun = Gun;
	//	}
	//}

	// This should be auto maintainable 
	// Auto cycling, until last gun - for reset
	int GunToEquipIndex = 0;
	for (int i = 0; i < GunInventory.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("GunInventory Num: %d"), GunInventory.Num());

		if (GunInventory[i] == nullptr) continue;
		bool bIsActive = GunInventory[i]->GetIsActive();
		UE_LOG(LogTemp, Warning, TEXT("bIsActive: %s , Index: %d"), bIsActive ? TEXT("true") : TEXT("false"), i);

		if (bIsActive)
		{
			UE_LOG(LogTemp, Warning, TEXT("Index Gun to Deactivate: %d"), i);
			GunInventory[i]->SheathAndDeactivate();
		}
		else if(!bIsActive && PreviousEquipIndex <= i)
		{
			PreviousEquipIndex = CurrentEquipIndex;
			GunToEquipIndex = i;
			CurrentEquipIndex = i;
			UE_LOG(LogTemp, Warning, TEXT("Index Changed Successfully : %d"), i);

			if (PreviousEquipIndex >= GunInventory.Num() - 1)
			{
				// Need to essentially re-route it to
				// invalid, so it can pick first gun
				PreviousEquipIndex = -1;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Index Gun to Activate: %d"), GunToEquipIndex);

	GunInventory[GunToEquipIndex]->WieldAndActivate();
	CurrentEquippedGun = GunInventory[GunToEquipIndex];
}

int AQuackCharacter::GetCurrentEquippedGunIndex() const
{
	return CurrentEquipIndex;
}

USkeletalMeshComponent* AQuackCharacter::GetSpecifcPawnMesh() const
{
	return Mesh1P;
}

FName AQuackCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

void AQuackCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpawnGunInventory();


}

// START
void AQuackCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//InitialiseGrabberClass();
	GetCharacterMovement()->MaxWalkSpeed = MovementConfig.InitialWalkSpeed;
	WeaponConfig.InitialFireCooldown = WeaponConfig.FireCooldown;
	PlayerConfig.MaxHealth = PlayerConfig.Health;
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	//CheckForMaxClip();

		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AQuackCharacter::OnTriggerEnter);
		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AQuackCharacter::OnTriggerExit);
}

// UPDATE
void AQuackCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Sprint();

		// Might change this to actual timer code
		SemiAutomaticShooting(DeltaTime);


}

// Interact

void AQuackCharacter::Interact()
{
	if (InteractableActor != nullptr)
	{
		// ID represents what you interacted with
		if (InteractID == 0)
		{
			// Gun stuff
			//WeaponConfig.bHasGravityGun = true;
			if (TutorialManager != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("TrialGravGunDone"));
				TutorialManager->TrialGravGunPickUp();
			}
		}
		else if (InteractID == 1)
		{
			// Needle stuff
			PoisonConfig.bHasNeedle = true;
			if (TutorialManager != nullptr)
			{
				TutorialManager->TrialPoisonUsed();
			}
		}
		InteractID = -1;
		InteractableActor->Destroy();
		InteractableActor = nullptr;
	}
}

// TRIGGERS
void AQuackCharacter::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	APipe* Pipe = Cast<APipe>(OtherActor);
	if (Pipe != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("CurrentPipeShouldbeassigned")));
		PlayerConfig.CurrentPipe = Pipe;
		if (PlayerConfig.CurrentPipe->bNotABossPipe)
		{
			PlayerConfig.CurrentPipe->ToggleHighlight(true);
		}
	}

	AHealthStation* Regen = Cast<AHealthStation>(OtherActor);
	if (Regen != nullptr)
	{
		Regen->SetIsInZone(true);
	}

	if (OtherActor != nullptr)
	{
		UInteractComponent* InteractComp = Cast<UInteractComponent>(OtherActor->GetComponentByClass(UInteractComponent::StaticClass()));
		if (InteractComp != nullptr)
		{
			InteractableActor = OtherActor;
			InteractID = InteractComp->GetID();
		}
	}
}

void AQuackCharacter::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		AHealthStation* Regen = Cast<AHealthStation>(OtherActor);
		if (Regen != nullptr)
		{
			Regen->SetIsInZone(false);
		}
		APipe* Pipe = Cast<APipe>(OtherActor);

		if (Pipe != nullptr)
		{
			// Just in case for overlapping triggers
			if (Pipe->bNotABossPipe)
			{
				Pipe->ToggleHighlight(false);
			}
			if (PlayerConfig.CurrentPipe != nullptr)
			{
				if (PlayerConfig.CurrentPipe->bNotABossPipe)
				{
					PlayerConfig.CurrentPipe->ToggleHighlight(false);
				}
				PlayerConfig.CurrentPipe = nullptr;
			}
		}
		
		UInteractComponent* InteractComp = Cast<UInteractComponent>(OtherActor->GetComponentByClass(UInteractComponent::StaticClass()));
		if (InteractComp != nullptr)
		{
			InteractableActor = nullptr;
			InteractID = -1;
		}
	}
}

// INPUT MANAGER
void AQuackCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("SwapGun", IE_Pressed, this, &AQuackCharacter::SwapGun);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AQuackCharacter::Reload);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AQuackCharacter::Interact);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AQuackCharacter::OnSprintBegin);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AQuackCharacter::OnSprintEnd);

	// See Tick for continuous shooting
	//
	PlayerInputComponent->BindAction("MouseDown", IE_Pressed, this, &AQuackCharacter::MouseDown);
	PlayerInputComponent->BindAction("MouseUp", IE_Released, this, &AQuackCharacter::MouseUp);

	PlayerInputComponent->BindAction("Poison", IE_Pressed, this, &AQuackCharacter::UsePoison);

	PlayerInputComponent->BindAxis("MoveForward", this, &AQuackCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AQuackCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AQuackCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AQuackCharacter::LookUpAtRate);
}

// GUN LOGIC
void AQuackCharacter::SwapGun()
{
	HandleChangeToNewGun();
}


void AQuackCharacter::IncreaseAmmo(int Amount)
{
	if (CurrentEquippedGun != nullptr)
	{
		// Handles max clip clamp inside
		CurrentEquippedGun->IncreaseAmmo(Amount);
	}
}

// POISON LOGIC
void AQuackCharacter::UsePoison()
{
	//UE_LOG(LogTemp, Warning, TEXT("Pressed E Poison"));

	if (PlayerConfig.CurrentPipe != nullptr && 	PoisonConfig.bHasNeedle)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Pipe exists and has needle"));

		if (PlayerConfig.CurrentPipe->bNotABossPipe)
		{
			//UE_LOG(LogTemp, Warning, TEXT("This Ran Poison"));
			PlayerConfig.CurrentPipe->bPoisonedPipe = true;
			PlayerConfig.CurrentPipe->bNotABossPipe = false;
			PlayerConfig.CurrentPipe->ChangeMeshColour();
			PlayerConfig.CurrentPipe->SimulateDestroy();
			PlayerConfig.CurrentPipe->ToggleHighlight(false);
			if (TutorialManager != nullptr)
			{
				PlayerConfig.CurrentPipe->OnPipeFinishedDraining.AddDynamic(TutorialManager, &ATutorialManager::RemoveAPipe);
			}
			//TutorialManager->RemoveAPipe();
			return;
		}
	}
	// Must have poison vials
	if (PoisonConfig.PoisonVialsCount == 0) return;
	// Cant be using poison already
	if (PoisonConfig.bIsPoisoning) return;
	// Must be within the pipe radius
	if (!PoisonConfig.bCanPoisonPipe) return;
	// Reduce vial count
	if (PlayerConfig.Boss != nullptr)
	{
		if (PlayerConfig.CurrentPipe != nullptr && PlayerConfig.Boss->TargettedPipe != nullptr)
		{
			if (PlayerConfig.CurrentPipe->GetName() == PlayerConfig.Boss->TargettedPipe->GetName())
			{
				if (PlayerConfig.Boss->CurrentBossState == BossStates::E_HealingOne || PlayerConfig.Boss->CurrentBossState == BossStates::E_HealingTwo || PlayerConfig.Boss->CurrentBossState == BossStates::E_HealingThree || PlayerConfig.Boss->CurrentBossState == BossStates::E_HealingFour)
				{
					PlayerConfig.CurrentPipe->bPoisonedPipe = true;
					PlayerConfig.CurrentPipe->ChangeMeshColour();
					PlayerConfig.CurrentPipe->SimulateDestroy();
					PlayerConfig.CurrentPipe->ToggleHighlight(false);
					PlayerConfig.Boss->ChangeState(BossStates::E_Poisoned);
					if (PlayerConfig.Boss->Pipes.Num() == 0)
					{
						PlayerConfig.Boss->TargettedPipe = nullptr;
					}
					PoisonConfig.PoisonVialsCount--;
					UWorld* const World = GetWorld();
					if (World != nullptr)
					{
						PoisonConfig.bIsPoisoning = true;
						World->GetTimerManager().SetTimer(PoisonConfig.PoisonUsageDelay, this, &AQuackCharacter::PoisonCooldown, PoisonConfig.PoisonCD, false);
					}
				}
			}

		} 
	}
}

void AQuackCharacter::PoisonCooldown()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		PoisonConfig.bIsPoisoning = false;
		World->GetTimerManager().ClearTimer(PoisonConfig.PoisonUsageDelay);
		if (PlayerConfig.Boss != nullptr)
		{
			//PlayerConfig.Boss->DestroyPreviousTargettedPipe();
			PlayerConfig.Boss->LocateNearbyPipe();
			PlayerConfig.Boss->ChangeState(BossStates::E_Idle);
		}
	}
}

// HEALTH LOGIC
void AQuackCharacter::IncreaseHealth(float Amount)
{
	PlayerConfig.Health += Amount ;
	PlayerConfig.Health = FMath::Clamp(PlayerConfig.Health, 0.0f, PlayerConfig.MaxHealth);
}

void AQuackCharacter::IncreaseHealth(float Amount, float DeltaTime)
{
	PlayerConfig.Health += Amount * DeltaTime;
	PlayerConfig.Health = FMath::Clamp(PlayerConfig.Health, 0.0f, PlayerConfig.MaxHealth);
}

void AQuackCharacter::DecreaseHealth(float Amount)
{
	PlayerConfig.Health -= Amount;
	if (PlayerConfig.Health <= 0.0f)
	{
		// You Dead
		UGameplayStatics::OpenLevel(this, FName("MainMenu"), false);
	}
	PlayerConfig.Health = FMath::Clamp(PlayerConfig.Health, 0.0f, PlayerConfig.MaxHealth);
}

void AQuackCharacter::Reload()
{
	if (CurrentEquippedGun != nullptr && !MovementConfig.bReloading)
	{
		float ReloadDuration = CurrentEquippedGun->Reload(Mesh1P);
		UWorld* const World = GetWorld();
		if (World == nullptr) return;
		FTimerHandle ReloadBoolHandle;
		if (ReloadDuration > 0.0f)
		{
			MovementConfig.bReloading = true;
			MovementConfig.bIsSprinting = false;
			World->GetTimerManager().SetTimer(ReloadBoolHandle, this, &AQuackCharacter::EndReload, ReloadDuration, false);
		}
		UE_LOG(LogTemp, Warning, TEXT("Initiaited Reload"));
	}
}

void AQuackCharacter::EndReload()
{
	MovementConfig.bReloading = false;
	if (MovementConfig.bSprintingCache)
	{
		OnSprintBegin();
	}
}

void AQuackCharacter::Raycast()
{
	// REMAKE THIS
	if (CurrentEquippedGun != nullptr && !MovementConfig.bIsSprinting)
	{
		CurrentEquippedGun->Shoot();
	}
	else
	{
		MovementConfig.bIsSprinting = false;
		// If you are sprinting and trying to shoot, make sure shooting stuff is reset
		if (CurrentEquippedGun != nullptr)
		{
			CurrentEquippedGun->StopMuzzleFlash();
		}
		WeaponConfig.RunningTime = 0.0f;
		WeaponConfig.FireCooldown = WeaponConfig.InitialFireCooldown;
	}
}

void AQuackCharacter::SemiAutomaticShooting(float DeltaTime)
{
	// NOTE: CHANGE TO TIMER

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (WeaponConfig.bMouseDown && !WeaponConfig.bMouseUp)
		{
			WeaponConfig.RunningTime += DeltaTime;
			if (WeaponConfig.RunningTime >= WeaponConfig.FireCooldown)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Shooting Active"));
				Raycast();
				WeaponConfig.FireCooldown += WeaponConfig.FireRate;
			}
			//World->GetTimerManager().SetTimer(ContinuousShooting, this, &AQuackCharacter::Raycast, 0.5f, true);
		}
		else
		{

			if (CurrentEquippedGun != nullptr)
			{
				CurrentEquippedGun->StopMuzzleFlash();
				//UE_LOG(LogTemp, Warning, TEXT("Shooting InActive"));
			}
			//StopMuzzleFlash();
			WeaponConfig.RunningTime = 0.0f;
			WeaponConfig.FireCooldown = WeaponConfig.InitialFireCooldown;
			//World->GetTimerManager().ClearTimer(ContinuousShooting); 
		}
	}
}

void AQuackCharacter::MouseDown()
{
	WeaponConfig.bMouseDown = true;
	WeaponConfig.bMouseUp = false;

}

void AQuackCharacter::MouseUp()
{
	WeaponConfig.bMouseDown = false;
	WeaponConfig.bMouseUp = true;
	if (MovementConfig.bSprintingCache)
	{
		OnSprintBegin();
	}
}

// MOVEMENT AND CAMERA TURN
void AQuackCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AQuackCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AQuackCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AQuackCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// SPRINTING
void AQuackCharacter::OnSprintBegin()
{
	// Very subtle bug, u could technically exploit, sprint bonus if u
	// jumped and reloaded then remained at sprint speed
	if (!MovementConfig.bReloading && !WeaponConfig.bMouseDown)
	{
		MovementConfig.bIsSprinting = true;
	}
	MovementConfig.bSprintingCache = true;
}

void AQuackCharacter::OnSprintEnd()
{
	MovementConfig.bIsSprinting = false;
	MovementConfig.bSprintingCache = false;
}

void AQuackCharacter::Sprint()
{
	if (MovementConfig.bIsSprinting)
	{
		//GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Green, FString::Printf(TEXT("Sprinting")));
		GetCharacterMovement()->MaxWalkSpeed = MovementConfig.SprintSpeed;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Green, FString::Printf(TEXT("Walking")));
		GetCharacterMovement()->MaxWalkSpeed = MovementConfig.InitialWalkSpeed;
	}
}

bool AQuackCharacter::IsSprinting() const
{
	return MovementConfig.bIsSprinting;
}