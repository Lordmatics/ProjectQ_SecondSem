// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/Elevator.h"
#include "Components/DestructibleComponent.h"
#include "Classes/PhysicsEngine/RadialForceComponent.h"
//#include "Headers/CustomComponents/VerticalMovementComponent.h"
#include "Headers/CustomComponents/MinionFactoryComponent.h"
#include "Headers/CustomComponents/BossArmourComponent.h"
#include "Headers/Boss/Armour/QuackArmourPin.h"
#include "Headers/Boss/Armour/QuackBossArmourBaseClass.h"
#include "Headers/CustomComponents/Matinee/MatineeContainerComponent.h"
#include "Headers/Boss/QuackBoss.h"
#include "Headers/Character/QuackCharacter.h"
#include "EngineUtils.h"

//#include "Engine.h"

// Sets default values
AElevator::AElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	ElevatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorMesh"));
	ElevatorMesh->SetupAttachment(MyRoot);

	LeftElevatorDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftElevatorDoor"));
	LeftElevatorDoor->SetupAttachment(MyRoot);

	RightElevatorDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightElevatorDoor"));
	RightElevatorDoor->SetupAttachment(MyRoot);

	SmashablePanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SmashablePanel"));
	SmashablePanel->SetupAttachment(MyRoot);

	SmashablePanelDM = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	SmashablePanelDM->OnComponentFracture.AddDynamic(this, &AElevator::OnComponentFracture);
	SmashablePanelDM->SetupAttachment(MyRoot);

	SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZone"));
	SpawnZone->SetupAttachment(MyRoot);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(SmashablePanelDM);

	// Might be simpler without this tbh
	//MovementComp = CreateDefaultSubobject<UVerticalMovementComponent>(TEXT("MovementComp"));
	MinionFactoryComp = CreateDefaultSubobject<UMinionFactoryComponent>(TEXT("MinionFactoryComp"));
	ArmourComp = CreateDefaultSubobject<UBossArmourComponent>(TEXT("ArmourComp"));
	CutsceneContainer = CreateDefaultSubobject<UMatineeContainerComponent>(TEXT("CutsceneContainer"));

}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	bGo = false;
	// Move Elevator to Initial Position
	SetActorLocation(FVector(-3382.0f, 17155.0f, ElevatorPositions.ElevatorStart), true);
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AQuackCharacter> ActorItr(World); ActorItr; ++ActorItr)
		{
			CharRef = *ActorItr;
		}
		
		FTimerHandle TempHandle;
		World->GetTimerManager().SetTimer(TempHandle, this, &AElevator::Start, 3.0f, false);
	}
}

void AElevator::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ArmourComp->PinClassUL != nullptr)
	{
		// LeftElevatorDoor->GetComponentLocation()
		AQuackArmourPin* PinUL = World->SpawnActor<AQuackArmourPin>(ArmourComp->PinClassUL, GetActorLocation(), GetActorRotation(), SpawnParams);
		if (PinUL != nullptr)
		{
			PinUL->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PinUL->SetActorRelativeScale3D(FVector(5.0f));
			PinUL->SetActorRelativeLocation(FVector(-650.0f, -200.0f, 1000.0f), false);
			PinUL->SetAlpha(0.0f);
		//	UE_LOG(LogTemp, Warning, TEXT("PinAlpha 0: %f"), PinUL->GetAlpha());
			PinRefUL = PinUL;
		}
	}
	if (ArmourComp->PinClassUR != nullptr)
	{
		AQuackArmourPin* PinUR = World->SpawnActor<AQuackArmourPin>(ArmourComp->PinClassUR, GetActorLocation(), GetActorRotation(), SpawnParams);
		if (PinUR != nullptr)
		{
			PinUR->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PinUR->SetActorRelativeScale3D(FVector(5.0f));
			PinUR->SetActorRelativeLocation(FVector(-200.0f, -200.0f, 1000.0f), false);
			PinUR->SetAlpha(0.0f);
			//UE_LOG(LogTemp, Warning, TEXT("PinAlpha 0: %f"), PinUR->GetAlpha());
			PinRefUR = PinUR;
		}
	}
}

void AElevator::Start()
{
	bGo = true;
}

// Called every frame
void AElevator::Tick( float DeltaTime)
{
	Super::Tick( DeltaTime );
	if (bGo)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BGO TRUE"));
		if (LeftElevatorDoor != nullptr && RightElevatorDoor != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Doors Not Null"));
			if (bOpenDoors)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Start Openning Doors"));

				if (LeftElevatorDoor->GetRelativeTransform().GetLocation().X >= DoorPositions.LeftDoorTargetX && RightElevatorDoor->GetRelativeTransform().GetLocation().X <= DoorPositions.RightDoorTargetX)
				{
					//	UE_LOG(LogTemp, Warning, TEXT("Doors Began Openning"));

					//LeftElevatorDoor->GetComponentLocation();
					float LeftX = LeftElevatorDoor->GetRelativeTransform().GetLocation().X;
					float LeftY = LeftElevatorDoor->GetRelativeTransform().GetLocation().Y;
					float LeftZ = LeftElevatorDoor->GetRelativeTransform().GetLocation().Z;

					LeftX = FMath::FInterpConstantTo(LeftX, DoorPositions.LeftDoorTargetX, DeltaTime, DoorSpeed);
					FVector NewLocationLeft = FVector(LeftX, LeftY, LeftZ);
					LeftElevatorDoor->SetRelativeLocation(FVector(NewLocationLeft), false);

					float RightX = RightElevatorDoor->GetRelativeTransform().GetLocation().X;
					float RightY = RightElevatorDoor->GetRelativeTransform().GetLocation().Y;
					float RightZ = RightElevatorDoor->GetRelativeTransform().GetLocation().Z;

					RightX = FMath::FInterpConstantTo(RightX, DoorPositions.RightDoorTargetX, DeltaTime, DoorSpeed);
					FVector NewLocationRight = FVector(RightX, RightY, RightZ);
					RightElevatorDoor->SetRelativeLocation(FVector(NewLocationRight), false);
				}
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Start Closing Doors"));

				if (LeftElevatorDoor->GetRelativeTransform().GetLocation().X <= DoorPositions.LeftDoorStartX && RightElevatorDoor->GetRelativeTransform().GetLocation().X >= DoorPositions.RightDoorStartX)
				{
					//	UE_LOG(LogTemp, Warning, TEXT("Doors Began Closing"));

					float LeftX = LeftElevatorDoor->GetRelativeTransform().GetLocation().X;
					float LeftY = LeftElevatorDoor->GetRelativeTransform().GetLocation().Y;
					float LeftZ = LeftElevatorDoor->GetRelativeTransform().GetLocation().Z;

					LeftX = FMath::FInterpConstantTo(LeftX, DoorPositions.LeftDoorStartX, DeltaTime, DoorSpeed);
					FVector NewLocationLeft = FVector(LeftX, LeftY, LeftZ);
					LeftElevatorDoor->SetRelativeLocation(FVector(NewLocationLeft), false);

					float RightX = RightElevatorDoor->GetRelativeTransform().GetLocation().X;
					float RightY = RightElevatorDoor->GetRelativeTransform().GetLocation().Y;
					float RightZ = RightElevatorDoor->GetRelativeTransform().GetLocation().Z;

					RightX = FMath::FInterpConstantTo(RightX, DoorPositions.RightDoorStartX, DeltaTime, DoorSpeed);
					FVector NewLocationRight = FVector(RightX, RightY, RightZ);
					RightElevatorDoor->SetRelativeLocation(FVector(NewLocationRight), false);
				}
			}
		}

		if (bElevatorStop)
		{
			if (MinionFactoryComp != nullptr)
			{
				// Once the spawned minions have all been slain
				// During the stop, Resume, movement
				if (!MinionFactoryComp->AreMinionsAlive())
				{
					UWorld* const World = GetWorld();
					if (World != nullptr)
					{
						if (!World->GetTimerManager().IsTimerActive(ResumeMovementHandle))
						{
							World->GetTimerManager().SetTimer(ResumeMovementHandle, this, &AElevator::ResumeMovement, 1.0f, false);
						}
					}
					//bElevatorStop = false;
					//if (!bDefeatedWave)
					//	bDefeatedWave = true;
					//else if (!bDefeatedBear)
					//	bDefeatedBear = true;
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("RETURNED: Shouldn't be moving"));
			FVector CurLocation = GetActorLocation();
			if (CurLocation.Z >= ElevatorPositions.ElevatorMinionTargetTwo)
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ElevatorPositions.ElevatorMinionTargetTwo), true);
			else if (CurLocation.Z >= ElevatorPositions.ElevatorMinionTargetOne)
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ElevatorPositions.ElevatorMinionTargetOne), true);


			return;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Moving"));
		// Handle Movement Upwards
		FVector MyLocation = GetActorLocation();
		float CurPos = MyLocation.Z;
		//UE_LOG(LogTemp, Warning, TEXT("CurPos: %f"), CurPos);
		//UE_LOG(LogTemp, Warning, TEXT("TargetPos: %f"), ElevatorPositions.ElevatorTarget);

		if (CurPos >= ElevatorPositions.ElevatorTarget)
		{
			CurPos = ElevatorPositions.ElevatorTarget;
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y, CurPos);
			SetActorLocation(NewLocation, true);

			if (PinRefUL != nullptr && PinRefUR != nullptr)
			{
				// Fade into reality once u reach the top
				PinRefUL->SetFade(true);
				PinRefUR->SetFade(true);
				if (!bPlayPinCutsceneOnce)
				{
					bPlayPinCutsceneOnce = true;
					if (CutsceneContainer != nullptr)
					{
						CutsceneContainer->PlayMatineeAt(0);
						float Duration = CutsceneContainer->GetMatineeLengthAt(0);
						if (CharRef != nullptr)
						{
							CharRef->Hide();
							FTimerHandle TempHandle;
							UWorld* const World = GetWorld();
							if (World != nullptr)
							{
								World->GetTimerManager().SetTimer(TempHandle, CharRef, &AQuackCharacter::UnHide, Duration, false);
							}
						}
					}
				}
				// Play Cutscene
				if (PinRefUL->bHasBeenDestroyed && PinRefUR->bHasBeenDestroyed)
				{
					if (LeftElevatorDoor != nullptr && RightElevatorDoor != nullptr && (PinRefUL->GetAlpha() > 0.5f || PinRefUR->GetAlpha() > 0.5f))
					{
						bOpenDoors = true;
						if (!bPlayDoorCutsceneOnce)
						{
							bPlayDoorCutsceneOnce = true;
							FTimerHandle Delay;
							UWorld* const World = GetWorld();
							if (World != nullptr)
							{
								World->GetTimerManager().SetTimer(Delay, this, &AElevator::DelayedActivation, DelayTime, false);
							}
							//if (CutsceneContainer != nullptr)
							//{
							//	CutsceneContainer->PlayMatineeAt(1);
							//	UWorld* const World = GetWorld();
							//	if (World != nullptr)
							//	{
							//		UGameplayStatics::GetAllActorsOfClass(World, AQuackBoss::StaticClass(), QuackBoss);
							//		if (QuackBoss.Num() > 0)
							//		{
							//			for (AActor* b : QuackBoss)
							//			{
							//				AQuackBoss* Boss = Cast<AQuackBoss>(b);
							//				if (Boss != nullptr)
							//				{
							//					Boss->SetCanMove();
							//				}
							//			}
							//		}
							//	}
							//}
						}
						// Change to open and close code
						//LeftElevatorDoor->DestroyComponent();
					}
					//if (RightElevatorDoor != nullptr)
					//{
					//	RightElevatorDoor->DestroyComponent();
					//}
				}
			}
		}
		else
		{
			CurPos = FMath::FInterpConstantTo(CurPos, ElevatorPositions.ElevatorTarget, DeltaTime, ElevatorSpeed);

			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y, CurPos);
			UE_LOG(LogTemp, Warning, TEXT("Rise"));
			SetActorLocation(NewLocation, true);
			if (CurPos >= ElevatorPositions.ElevatorMinionTargetOne && !bDefeatedWave)
			{
				// Play Camera Shake
				if (MinionFactoryComp != nullptr)
				{
					if (SpawnZone != nullptr)
					{
						if (MinionFactoryComp->SpawnElevatorWave(EnemiesToSpawn, SpawnZone))
						{
							if (RadialForce != nullptr)
							{
								RadialForce->FireImpulse();
							}
							bElevatorStop = true;
						}
					}
				}
			}
			else if (CurPos >= ElevatorPositions.ElevatorMinionTargetTwo && !bDefeatedBear)
			{
				// Play Camera Shake
				if (MinionFactoryComp != nullptr)
				{
					if (SpawnZone != nullptr)
					{
						if (MinionFactoryComp->SpawnBear(SpawnZone))
						{
							bElevatorStop = true;
						}
					}
				}
			}
		}
		//NewHeight = FMath::Clamp(NewHeight, ElevatorPositions.ElevatorStart, ElevatorPositions.ElevatorTarget);

	}
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Elevator Stop: %s"), bElevatorStop ? TEXT("true") : TEXT("false")));
	//	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("bDefeatedWave: %s"), bDefeatedWave ? TEXT("true") : TEXT("false")));
	//	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("bDefeatedBear: %s"), bDefeatedBear ? TEXT("true") : TEXT("false")));
	//	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("MinionsAlive: %s"), MinionFactoryComp->AreMinionsAlive() ? TEXT("true") : TEXT("false")));


	//}
	// Elevator Stop true, once reach a certain pos
	
}

void AElevator::DelayedActivation()
{
	if (CutsceneContainer != nullptr)
	{
		CutsceneContainer->PlayMatineeAt(1);
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			float Duration = CutsceneContainer->GetMatineeLengthAt(1);
			if (CharRef != nullptr)
			{
				CharRef->Hide();
				FTimerHandle TempHandle;
				World->GetTimerManager().SetTimer(TempHandle, CharRef, &AQuackCharacter::UnHide, Duration, false);		
			}
			UGameplayStatics::GetAllActorsOfClass(World, AQuackBoss::StaticClass(), QuackBoss);
			if (QuackBoss.Num() > 0)
			{
				for (AActor* b : QuackBoss)
				{
					AQuackBoss* Boss = Cast<AQuackBoss>(b);
					if (Boss != nullptr)
					{
						Boss->SetCanMove();
					}
				}
			}
		}
	}
}

void AElevator::ResumeMovement()
{
	bElevatorStop = false;
	if (!bDefeatedWave)
		bDefeatedWave = true;
	else if (!bDefeatedBear)
		bDefeatedBear = true;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(ResumeMovementHandle);
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC != nullptr)
		{
			PC->ClientPlayCameraShake(ElevatorShake);
		}
	}
}

void AElevator::OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if(SmashablePanel != nullptr)
			SmashablePanel->DestroyComponent();
		
		FTimerHandle CollisionTimer;
		World->GetTimerManager().SetTimer(CollisionTimer, this, &AElevator::DisableCollision, CollisionTimerDelay, false);

		FTimerHandle DestroyTimer;
		World->GetTimerManager().SetTimer(DestroyTimer, this, &AElevator::DestroyThis, DestroyPanelDelay, false);
	}
}

void AElevator::DisableCollision()
{
	if (SmashablePanelDM != nullptr)
	{
		//MyDestructibleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SmashablePanelDM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AElevator::DestroyThis()
{
	if(SmashablePanelDM != nullptr)
		SmashablePanelDM->DestroyComponent();
}

void AElevator::CloseDoors()
{
	bOpenDoors = false;
}