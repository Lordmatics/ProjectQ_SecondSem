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
#include "Headers/Character/Guns/BurstRifle.h"
#include "Headers/Character/Guns/Needle.h"
#include "Classes/Components/PostProcessComponent.h"
#include "Animation/AnimMontage.h"
#include "Headers/Misc/QuackGameMode.h"

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
	bCanFire = true;
}

float AQuackCharacter::ChangeGunAnimation(int GunIndex)
{
	// Swapping to needle
	if (GunIndex == 3)
	{
		bSwapToNeedle = true;
		bSwapToGun = false;
	}
	// Swapping to normal gun
	else
	{
		bSwapToNeedle = false;
		bSwapToGun = true;
	}
	bCanFire = false;
	UWorld* const World = GetWorld();
	if (World == nullptr || RaiseGunAnimation == nullptr || LowerGunAnimation == nullptr) return 0.0f;
	float Duration = /*RaiseGunAnimation->GetPlayLength()*/ LowerGunAnimation->GetPlayLength();
	FTimerHandle TempHandle;
	World->GetTimerManager().SetTimer(TempHandle, this, &AQuackCharacter::ResetGunSwapBools, Duration - 0.5f, false);
	return Duration;
	//if (LowerGunAnimation != nullptr)
	//{
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if (AnimInstance != nullptr)
	//	{
	//		float LowerDuration = AnimInstance->Montage_Play(LowerGunAnimation, 1.0f);
	//		FTimerHandle TempHandle;
	//		World->GetTimerManager().SetTimer(TempHandle, this, &AQuackCharacter::EquipNewGunAnim, LowerDuration, false);
	//		bCanFire = false;
	//	}
	//}
}

void AQuackCharacter::ResetGunSwapBools()
{
	bSwapToNeedle = false;
	bSwapToGun = false;
	//bCanFire = true;
}

void AQuackCharacter::EquipNewGunAnim()
{
	AllowFiring();
	//UWorld* const World = GetWorld();
	//if (World == nullptr) return;
	//if (RaiseGunAnimation != nullptr)
	//{
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if (AnimInstance != nullptr)
	//	{
	//		float RaiseDuration = RaiseGunAnimation->GetPlayLength();
	//		//AnimInstance->Montage_Play(RaiseGunAnimation, 1.0f);

	//		FTimerHandle TempHandle;
	//		World->GetTimerManager().SetTimer(TempHandle, this, &AQuackCharacter::AllowFiring, RaiseDuration, false);
	//	}
	//}
}

void AQuackCharacter::AllowFiring()
{
	bCanFire = true;
}

void AQuackCharacter::SpawnGunInventory()
{

	UWorld* const World = GetWorld();
	// Early exits - should never want this to occur
	if (World == nullptr) return;
	if (GunInventoryClasses.Num() == 0) return;
	for (size_t i = 0; i < GunInventoryClasses.Num(); i++)
	{
		if (GunInventoryClasses[i] == nullptr) return;
	}

	//if (GunInventoryClasses[0] == nullptr || GunInventoryClasses[1] == nullptr) return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (GunInventoryClasses.Num() < 1) return;
	AAssaultRifle* AR = World->SpawnActor<AAssaultRifle>(GunInventoryClasses[0], SpawnInfo);
	if (AR != nullptr)
	{


		AssaultRifleRef = AR;
		AssaultRifleRef->SetOwningPawn(this);
		AssaultRifleRef->WieldAndActivate();
		CurrentEquipIndex = 0;
		CurrentEquippedGun = AssaultRifleRef;
		// Required for Raycast
		if(FirstPersonCameraComponent != nullptr)
			AssaultRifleRef->SetOwningCamera(FirstPersonCameraComponent);
		GunInventory.Add(AssaultRifleRef);
	}
	if (GunInventoryClasses.Num() < 2) return;
	APlasmaRifle* PR = World->SpawnActor<APlasmaRifle>(GunInventoryClasses[1], SpawnInfo);
	if (PR != nullptr)
	{
		PlasmaRifleRef = PR;
		PlasmaRifleRef->SetOwningPawn(this);
		PlasmaRifleRef->SheathAndDeactivate();
		GunInventory.Add(PlasmaRifleRef);
	}
	if (GunInventoryClasses.Num() < 3) return;
	ABurstRifle* BR = World->SpawnActor<ABurstRifle>(GunInventoryClasses[2], SpawnInfo);
	if (BR != nullptr)
	{
		BurstRifleRef = BR;
		BurstRifleRef->SetOwningPawn(this);
		BurstRifleRef->SheathAndDeactivate();
		GunInventory.Add(BurstRifleRef);
	}
	if (GunInventoryClasses.Num() < 4) return;
	ANeedle* Needle = World->SpawnActor<ANeedle>(GunInventoryClasses[3], SpawnInfo);
	if (Needle != nullptr)
	{
		bHasNeedleEquipped = false;
		NeedleRef = Needle;
		NeedleRef->SetOwningPawn(this);
		NeedleRef->SheathAndDeactivate();
		GunInventory.Add(NeedleRef);
	}
}

void AQuackCharacter::ForceNeedleGun()
{
	//for (size_t i = 0; i < GunInventory.Num(); i++)
	//{
	//	if (GunInventory[i] == nullptr) continue;
	//	//UE_LOG(LogTemp, Warning, TEXT("Gun should be inactive: %s"), GunInventory[i]->GetIsActive() ? TEXT("GunACtive") : TEXT("GunInactive"));
	//	GunInventory[i]->SheathAndDeactivate();
	//	//UE_LOG(LogTemp, Warning, TEXT("Gun should still be inactive: %s"), GunInventory[i]->GetIsActive() ? TEXT("GunACtive") : TEXT("GunInactive"));
	//}
	//GunInventory[CurrentEquipIndex]->SheathAndDeactivate();
	
	//GunInventory[3]->WieldAndActivate();
	//CurrentEquippedGun = GunInventory[3];


	UWorld* const World = GetWorld();
	if (World == nullptr) return;


	//GunInventory[i]->SheathAndDeactivate();

	for (size_t i = 0; i < GunInventory.Num(); i++)
	{
		if (GunInventory[i] == nullptr) continue;
		GunInventory[i]->DisableGun();
		bool bIsActive = GunInventory[i]->GetIsActive();
		if (bIsActive)
		{
			// HIDE EQUIPPED GUN
			FTimerHandle TempHan;
			FTimerDelegate TempDelegate;
			TempDelegate.BindUFunction(this, FName("HideCurrentGun"), i);
			World->GetTimerManager().SetTimer(TempHan, TempDelegate, 0.6f, false);
			float Duration = ChangeGunAnimation(i);
			break;
		}
		else
		{
			GunInventory[i]->SheathAndDeactivate();
		}
	}
	// REVEAL NEW GUN
	FTimerHandle TempHandle;
	FTimerDelegate TempDel;
	TempDel.BindUFunction(this, FName("FinishSwapping"), 3);
	World->GetTimerManager().SetTimer(TempHandle, TempDel, 0.65f, false);
	CurrentEquipIndex = 3;
	//bHasNeedleEquipped = true;
	CanSwapGun = false;
}

void AQuackCharacter::UnforceNeedleGun()
{
	CanSwapGun = true;
	for (size_t i = 0; i < GunInventory.Num(); i++)
	{
		if (GunInventory[i] == nullptr) continue;
		GunInventory[i]->EnableGun();
	}
}

void AQuackCharacter::HandleChangeToNewGun()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
		if (CurrentEquippedGun != nullptr)
		{
			// Alternatively, can cancel reload and force swap
			// Discuss what we want
			if (CurrentEquippedGun->GetIsReloading() || CurrentEquippedGun->InUseUnableToSwap())
			{
				UE_LOG(LogTemp, Warning, TEXT("Tried to Swap Gun but was shut down"));

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


		int GunIndexToEquip = 0;
		for (size_t i = 0; i < GunInventory.Num(); i++)
		{
			if (GunInventory[i] == nullptr) continue;
			bool bIsActive = GunInventory[i]->GetIsActive();
			if (bIsActive)
			{
				if (i == GunInventory.Num() - 1)
				{
					GunIndexToEquip = 0;
					CurrentEquipIndex = 0;
				}
				else
				{
					GunIndexToEquip = i + 1;
					CurrentEquipIndex = i + 1;
				}
				
				// HIDE EQUIPPED GUN 
				bLowerGun = true;
				FTimerHandle TempHan;
				FTimerDelegate TempDelegate;
				TempDelegate.BindUFunction(this, FName("HideCurrentGun"), i);
				// 0.6f;
				World->GetTimerManager().SetTimer(TempHan, TempDelegate, GunInventory[i]->GetDelayTillOffscreen(), false);

				//GunInventory[i]->SheathAndDeactivate();
				// REVEAL NEW GUN
				float Duration = ChangeGunAnimation(i);
				FTimerHandle TempHandle;
				FTimerDelegate TempDel;
				TempDel.BindUFunction(this, FName("FinishSwapping"), GunIndexToEquip);
				World->GetTimerManager().SetTimer(TempHandle, TempDel, GunInventory[i]->GetDelayTillOffscreen() + 0.05f, false);
				break;
			}
			else
			{
				GunInventory[i]->SheathAndDeactivate();
			}
		//FinishSwapping(GunIndexToEquip);
		//GunInventory[GunIndexToEquip]->WieldAndActivate();
		//CurrentEquippedGun = GunInventory[GunIndexToEquip];
		//if (CurrentEquippedGun == GunInventory[3])
		//{
		//	bHasNeedleEquipped = true;
		//}
		//else
		//{
		//	bHasNeedleEquipped = false;
		//}
	}


	//int GunToEquipIndex = 0;
	//for (int i = 0; i < GunInventory.Num(); i++)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("GunInventory Num: %d"), GunInventory.Num());

	//	if (GunInventory[i] == nullptr) continue;
	//	bool bIsActive = GunInventory[i]->GetIsActive();
	//	UE_LOG(LogTemp, Warning, TEXT("bIsActive: %s , Index: %d"), bIsActive ? TEXT("true") : TEXT("false"), i);

	//	if (bIsActive)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Index Gun to Deactivate: %d"), i);
	//		GunInventory[i]->SheathAndDeactivate();
	//	}
	//	else if(!bIsActive && PreviousEquipIndex <= i)
	//	{
	//		PreviousEquipIndex = CurrentEquipIndex;
	//		GunToEquipIndex = i;
	//		CurrentEquipIndex = i;
	//		UE_LOG(LogTemp, Warning, TEXT("Index Changed Successfully : %d"), i);

	//		if (PreviousEquipIndex >= GunInventory.Num() - 1)
	//		{
	//			// Need to essentially re-route it to
	//			// invalid, so it can pick first gun
	//			PreviousEquipIndex = -1;
	//		}
	//		break;
	//	}
	//}
	//UE_LOG(LogTemp, Warning, TEXT("Index Gun to Activate: %d"), GunToEquipIndex);

	//GunInventory[GunToEquipIndex]->WieldAndActivate();
	//CurrentEquippedGun = GunInventory[GunToEquipIndex];
}

void AQuackCharacter::HideCurrentGun(int Index)
{
	if (GunInventory[Index] != nullptr)
	{
		GunInventory[Index]->SheathAndDeactivate();
		bLowerGun = false;
		bRaiseGun = true;
		AnimationEquipIndex = CurrentEquipIndex;
	}

}

void AQuackCharacter::FinishSwapping(int Index)
{
	GunInventory[Index]->WieldAndActivate();
	CurrentEquippedGun = GunInventory[Index];
	if (CurrentEquippedGun == GunInventory[3])
	{
		bHasNeedleEquipped = true;
	}
	else
	{
		bHasNeedleEquipped = false;
	}
	bCanFire = true;
	bRaiseGun = false;
	bLowerGun = false;
}

int AQuackCharacter::GetCurrentEquippedGunIndex() const
{
	return CurrentEquipIndex;
}

USkeletalMeshComponent* AQuackCharacter::GetSpecificPawnMesh() const
{
	return Mesh1P;
}

FName AQuackCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

FName AQuackCharacter::GetWeaponAttachPointLaser() const
{
	return WeaponAttachPointLaser;
}

FName AQuackCharacter::GetWeaponAttachPointPlasma() const 
{
	return WeaponAttachPointPlasma;
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

		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("PlayerMovement Restricted: %s"), bMovementPrevented ? TEXT("True") : TEXT("False")));
		//}
		if (GetPlayerMovement())
		{
			// This doesn't want to work, even though its the same rotating logic as the boss
			//RotateTowardsTargettedPipe(DeltaTime);
		}


}

void AQuackCharacter::RotateTowardsTargettedPipe(float DeltaTime)
{
	if (PlayerConfig.Boss->TargettedPipe != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("RotatingToPipe"));
		FRotator MyRotation = GetActorRotation();
		const FVector MyLocation = GetActorLocation();
		//// Get other transform
		const FVector OtherLocation = PlayerConfig.Boss->TargettedPipe->GetLookAtLocation();
		//// Find vector that connects the transforms
		FVector Direction = MyLocation - OtherLocation;
		// Project it to 2D
		Direction = FVector(Direction.X, Direction.Y, 0.0f);
		FRotator EndRotation = Direction.Rotation();
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Red, FString::Printf(TEXT("Rotation: %s, EndRotation: %s"), *MyRotation.ToString(), *EndRotation.ToString()));
		}*/
		//UE_LOG(LogTemp,Warning,TEXT("Rotation: %s, EndRotation: %s"), *MyRotation.ToString(), *EndRotation.ToString());
		//MyRotation = FMath::RInterpConstantTo(MyRotation, EndRotation, DeltaTime, RotationSpeed);
		//SetActorRotation(MyRotation);

		// calculate delta for this frame from the rate information
		//if (EndRotation.Yaw > MyRotation.Yaw + 10.0f)
		//{
		//	AddControllerYawInput(-3.0f * BaseTurnRate * DeltaTime);
		//}
		//else if (EndRotation.Yaw < MyRotation.Yaw - 10.0f)
		//{
		//	AddControllerYawInput(3.0f * BaseTurnRate * DeltaTime);
		//}
		//MyRotation.Yaw = FMath::Clamp(MyRotation.Yaw, 0.0f, -180.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IN FUNCTION BUT NOT ROTATING"));
	}
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
	if (CurrentEquipIndex == 3)
	{
		if (PlayerConfig.CurrentPipe != nullptr && 	PoisonConfig.bHasNeedle)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Pipe exists and has needle"));

			//if (NeedleRef != nullptr)
			//{
			//	UAnimMontage* StabAnim = NeedleRef->GetNeedleStabAnimation();
			//	if (StabAnim != nullptr)
			//	{
			//		PlayAnimMontage(StabAnim, 1.0f);
			//		UE_LOG(LogTemp, Warning, TEXT("Animation Should Play : Stab"));
			//	}
			//}

			if (PlayerConfig.CurrentPipe->bNotABossPipe)
			{
				//UE_LOG(LogTemp, Warning, TEXT("This Ran Poison"));
				PlayerConfig.CurrentPipe->bPoisonedPipe = true;
				//PlayerConfig.CurrentPipe->
				PlayerConfig.CurrentPipe->bNotABossPipe = false;
				// Need to turn it off - to prevent stabbing a poisonned pipe animation
				PoisonConfig.bCanPoisonPipe = false;
				PlayerConfig.CurrentPipe->ChangeMeshColour();
				PlayerConfig.CurrentPipe->SimulateDestroy();
				PlayerConfig.CurrentPipe->ToggleHighlight(false);
				if (NeedleRef != nullptr)
				{
					NeedleRef->PlayStabAnimation();
				}

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
					if (PlayerConfig.Boss->CurrentBossState == BossStates::E_HealingOne || PlayerConfig.Boss->CurrentBossState == BossStates::E_HealingTwo
						|| PlayerConfig.Boss->CurrentBossState == BossStates::E_HealingThree || PlayerConfig.Boss->CurrentBossState == BossStates::E_HealingFour)
					{
						//if (NeedleRef != nullptr)
						//{
						//	UAnimMontage* StabAnim = NeedleRef->GetNeedleStabAnimation();
						//	if (StabAnim != nullptr)
						//	{
						//		PlayAnimMontage(StabAnim, 1.0f);
						//		UE_LOG(LogTemp, Warning, TEXT("Animation Should Play : Stab"));
						//	}
						//}
						if (NeedleRef != nullptr)
						{
							// Cannot Move
							SetPlayerMovement(true);
							//UE_LOG(LogTemp, Warning, TEXT("PlayerCanMoveAgain: %s"), bMovementPrevented ? TEXT("CANMOVE") : TEXT("CANNOTMOVE"));
							NeedleRef->PlayStabAnimation();
						}
						UWorld* TempWorld = GetWorld();
						if (TempWorld != nullptr)
						{
							AQuackGameMode* TempGameMode = Cast<AQuackGameMode>(TempWorld->GetAuthGameMode());
							if (TempGameMode != nullptr)
							{
								float RealtimeSeconds = UGameplayStatics::GetRealTimeSeconds(TempWorld);
								FString TempString = TEXT("Used needle at: ");
								TempString += FString::FromInt(RealtimeSeconds);
								TempString += LINE_TERMINATOR;
								TempGameMode->AddToString(TempString);
							}
						}
						PlayerConfig.CurrentPipe->bPoisonedPipe = true;
						PlayerConfig.CurrentPipe->ChangeMeshColour();
						PlayerConfig.CurrentPipe->SimulateDestroy();
						PlayerConfig.CurrentPipe->ToggleHighlight(false);
						// Need to turn it off - to prevent stabbing a poisonned pipe animation
						PoisonConfig.bCanPoisonPipe = false;
						// Force boss to go to next state
						PlayerConfig.Boss->ChangeState(BossStates::E_Poisoned);
						// No Pipes left, set TargettedPipe - NULL
						if (PlayerConfig.Boss->LowerPipes.Num() == 0 && PlayerConfig.Boss->UpperPipes.Num() == 0)
						{
							PlayerConfig.Boss->TargettedPipe = nullptr;
						}
						PoisonConfig.PoisonVialsCount--;
						UWorld* const World = GetWorld();
						if (World != nullptr)
						{
							if(!World->GetTimerManager().IsTimerActive(DelayRecoilHandle))
								World->GetTimerManager().SetTimer(DelayRecoilHandle, this, &AQuackCharacter::DelayRecoil, DelayRecoilDuration, false);
							//PoisonConfig.bIsPoisoning = true;
							World->GetTimerManager().SetTimer(PoisonConfig.PoisonUsageDelay, this, &AQuackCharacter::PoisonCooldown, PoisonConfig.PoisonCD, false);
						}
					}
				}

			}
		}
	}
}

void AQuackCharacter::DelayRecoil()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().ClearTimer(DelayRecoilHandle);
	PoisonConfig.bIsPoisoning = true;
	// Regain Movement - Might wanna delay another second to finish anim
	
	if (!World->GetTimerManager().IsTimerActive(RegainMovementHandle))
	{
		World->GetTimerManager().SetTimer(RegainMovementHandle, this, &AQuackCharacter::RegainMovement, 1.3f, false);
	}
	//SetPlayerMovement(false);
}

void AQuackCharacter::RegainMovement()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().ClearTimer(RegainMovementHandle);
	SetPlayerMovement(false);
	//UE_LOG(LogTemp, Warning, TEXT("PlayerCanMoveAgain: %s"), bMovementPrevented ? TEXT("CANMOVE") : TEXT("CANNOTMOVE"));
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
void AQuackCharacter::SetInBile()
{
	bInBile = true;
}

void AQuackCharacter::OutOfBileEffect()
{
	bInBile = false;
}

void AQuackCharacter::Flash()
{
	bGotHit = false;
}

void AQuackCharacter::DecreaseHealth(float Amount)
{
	PlayerConfig.Health -= Amount;
	if (!bGotHit && !bInBile)
	{
		bGotHit = true;
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//if(HitCameraShake != nullptr)
			//	UGameplayStatics::PlayWorldCameraShake(World, HitCameraShake, HitEpicentre, InnerRadius, OuterRadius, Falloff, bOrientShakeTowardsEpicentre);
			APlayerController* PC = Cast<APlayerController>(GetController());
			if (PC != nullptr)
			{
				if (HitCameraShake != nullptr)
					PC->ClientPlayCameraShake(HitCameraShake);
			}
			FTimerHandle ResetHandle;
			World->GetTimerManager().SetTimer(ResetHandle, this, &AQuackCharacter::Flash, FlashDelay * 2, false);
		}
	}
	if (PlayerConfig.Health <= 0.0f)
	{
		// You Dead
		UWorld* TempWorld = GetWorld();
		if (TempWorld != nullptr)
		{
			AQuackGameMode* TempGameMode = Cast<AQuackGameMode>(TempWorld->GetAuthGameMode());
			if (TempGameMode != nullptr)
			{
				float RealtimeSeconds = UGameplayStatics::GetRealTimeSeconds(TempWorld);
				FString TempString = TEXT("Died at: ");
				TempString += FString::FromInt(RealtimeSeconds);
				TempString += LINE_TERMINATOR;
				TempGameMode->AddToString(TempString);
				TempGameMode->WriteToFile();
			}
		}
		UGameplayStatics::OpenLevel(this, FName("MainMenu"), false);
	}
	PlayerConfig.Health = FMath::Clamp(PlayerConfig.Health, 0.0f, PlayerConfig.MaxHealth);
}

void AQuackCharacter::Reload()
{
	if (CurrentEquippedGun != nullptr && !MovementConfig.bReloading && Mesh1P != nullptr)
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
		//UE_LOG(LogTemp, Warning, TEXT("Initiaited Reload"));
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
	if (!bCanFire || !CanSwapGun) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (WeaponConfig.bMouseDown && !WeaponConfig.bMouseUp)
		{
			WeaponConfig.RunningTime += DeltaTime;
			// weapon config, needs to change to currentequippedgun->rate etc
			// To allow new guns
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
				if (CurrentEquippedGun->IsFullyAutomatic())
				{
					CurrentEquippedGun->StopMuzzleFlash();
				}
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
	if (CurrentEquippedGun == BurstRifleRef)
	{
		BurstRifleRef->Blast();
	}
	if (MovementConfig.bSprintingCache)
	{
		OnSprintBegin();
	}
}

// MOVEMENT AND CAMERA TURN
void AQuackCharacter::MoveForward(float Value)
{
	if (bMovementPrevented) return;
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AQuackCharacter::MoveRight(float Value)
{
	if (bMovementPrevented) return;

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AQuackCharacter::TurnAtRate(float Rate)
{
	if (bMovementPrevented) return;

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AQuackCharacter::LookUpAtRate(float Rate)
{
	if (bMovementPrevented) return;

	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// SPRINTING
void AQuackCharacter::OnSprintBegin()
{
	// Very subtle bug, u could technically exploit, sprint bonus if u
	// jumped and reloaded then remained at sprint speed
	if (!MovementConfig.bReloading && !WeaponConfig.bMouseDown)// && (GetVelocity().X > 0.0f))
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
	if (bMovementPrevented) return;

	if (MovementConfig.bIsSprinting )
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

