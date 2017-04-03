
#include "Headers/Quack.h"
#include "Headers/Boss/QuackBoss.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/Misc/Pipe.h"
#include "Headers/Misc/RockSpawner.h"
#include "Headers/Misc/Rock.h"
#include "Headers/Misc/QuackProjectile.h"
#include "Headers/Misc/DestructibleInnerPipes.h"
#include "Headers/CustomComponents/MinionFactoryComponent.h"
#include "Components/ArrowComponent.h"
#include "Headers/Audio/AudioManager.h"
#include "EngineUtils.h"
#include "Headers/CustomComponents/VerticalMovementComponent.h"
#include "Headers/CustomComponents/AnimationComponent.h"
#include "Headers/CustomComponents/BossAttacksComponent.h"
#include "Classes/Animation/AnimInstance.h"
#include "Headers/CustomComponents/BossArmourComponent.h"
#include "Headers/Boss/Armour/QuackArmourBody.h"
#include "Headers/Boss/Armour/QuackArmourPin.h"
#include "Headers/Boss/Armour/QuackBossArmourBaseClass.h"
#include "Headers/CustomComponents/RaycastComponent.h"
#include "Classes/Animation/AnimMontage.h"
#include "Headers/CustomComponents/Matinee/MatineeContainerComponent.h"
#include "Headers/CustomComponents/SmashByBeamComponent.h"
#include "Headers/Misc/QuackGameMode.h"

// Sets default values
AQuackBoss::AQuackBoss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AnimatedMesh"));
	//SkeletalMesh->SetupAttachment(MyRoot);

	//ProjectileSpawn = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawn"));
	//ProjectileSpawn->SetupAttachment(MyRoot);

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	Chandelier = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chandelier"));
	Chandelier->SetupAttachment(MyRoot);

	MySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MySkeletalMesh"));
	MySkeletalMesh->SetupAttachment(Chandelier);

	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(MySkeletalMesh);

	TailArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TailArrow"));
	TailArrow->SetupAttachment(MySkeletalMesh);
	//TailArrow->AttachTo(MySkeletalMesh, FName("Tail6"), EAttachLocation::SnapToTarget);

	BodyPlate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyPlate"));
	BodyPlate->SetWorldScale3D(FVector(200.0f));
	BodyPlate->SetRelativeLocation(FVector(-0.002449f, 56.249912f, 827.373047f));
	BodyPlate->SetRelativeRotation(FRotator(40.0f, 0.0f, 0.0f));
	//BodyPlate->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	BodyPlate->SetupAttachment(MySkeletalMesh);

	BodyUL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyUL"));
	BodyUL->SetWorldScale3D(FVector(1.0f));
	BodyUL->SetVisibility(false);
	BodyUL->SetupAttachment(BodyPlate);

	BodyUR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyUR"));
	BodyUR->SetWorldScale3D(FVector(1.0f));
	BodyUR->SetVisibility(false);
	BodyUR->SetupAttachment(BodyPlate);

	BodyLL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyLL"));
	BodyLL->SetWorldScale3D(FVector(1.0f));
	BodyLL->SetVisibility(false);
	BodyLL->SetupAttachment(BodyPlate);

	BodyLR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyLR"));
	BodyLR->SetWorldScale3D(FVector(1.0f));
	BodyLR->SetVisibility(false);
	BodyLR->SetupAttachment(BodyPlate);

	LaserCannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserCannon"));
	LaserCannon->SetWorldScale3D(FVector(100.0f));
	LaserCannon->SetRelativeLocation(FVector(-0.002477f, 54.748207f, 832.158630f));
	LaserCannon->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	LaserCannon->SetupAttachment(MySkeletalMesh);
	LaserBuildup = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserBuildUp"));
	LaserBuildup->SetupAttachment(LaserCannon);
	LaserParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserPS"));
	LaserParticleSystemComp->SetupAttachment(LaserCannon);
	LaserParticleSystemComp->SetRelativeScale3D(FVector(0.0015f));
	LaserParticleSystemComp->SetActive(false);
	LaserCannon->SetHiddenInGame(true);

	EMPParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EMPParticle"));
	EMPParticleSystem->SetupAttachment(RootComponent);

	//BodyPlate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT(""))

	MouthArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MouthArrow"));
	MouthArrow->SetupAttachment(MySkeletalMesh);
	//MouthArrow->AttachTo(MySkeletalMesh, FName("Mouth"), EAttachLocation::SnapToTarget);

	//TailArrow->AttachToComponent(MySkeletalMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Tail6"));
	////TailArrow->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));
	////	TailArrow->SetRelativeLocation(FVector(0.0f, 0.0f , 50.0f));

	//MouthArrow->AttachToComponent(MySkeletalMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Mouth"));
	////MouthArrow->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));
	////	MouthArrow->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	//InitialiseRockSpawnerReference();

	MinionFactory = CreateDefaultSubobject<UMinionFactoryComponent>(TEXT("MinionFactory"));
	ChandelierDropComponent = CreateDefaultSubobject<UVerticalMovementComponent>(TEXT("ChandelierDropComponent"));
	AnimationComponent = CreateDefaultSubobject<UAnimationComponent>(TEXT("Animation Component"));
	BossAttacksComponent = CreateDefaultSubobject<UBossAttacksComponent>(TEXT("BossAttacksComponent"));
	BossArmourComponent = CreateDefaultSubobject<UBossArmourComponent>(TEXT("BossArmourComponent"));
	RaycastComponent = CreateDefaultSubobject<URaycastComponent>(TEXT("RaycastComponent"));
	CutsceneContainer = CreateDefaultSubobject<UMatineeContainerComponent>(TEXT("MatineeContainer"));

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AAudioManager> ActorItr(World); ActorItr; ++ActorItr)
		{
			AudioManager = *ActorItr;
		}
	}
	InitialisePlayerCharacterReference();
}

void AQuackBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Delegate collision onto the boss mesh
	MySkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &AQuackBoss::OnTriggerEnter);
	if (BodyPlate != nullptr)
	{
		FAttachmentTransformRules TransformRules = FAttachmentTransformRules::KeepWorldTransform;
		//TransformRules.ScaleRule = EAttachmentRule::KeepWorld;
		BodyPlate->AttachToComponent(MySkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), FName("ArmourSocket"));
		BodyPlate->SetVisibility(false, true);
	}

	// TODO: MAKE CANNON SKELETAL MESH WITH SOCKETS
	//if (LaserParticleSystemComp != nullptr && LaserCannon != nullptr)
	//	LaserParticleSystemComp->AttachToComponent(LaserCannon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LaserSocket"));

	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (BossArmourComponent != nullptr)
	{
		if (BossArmourComponent->MainBodyArmourClass != nullptr)
		{
			AQuackBossArmourBaseClass* MainBody = World->SpawnActor<AQuackBossArmourBaseClass>(BossArmourComponent->MainBodyArmourClass, BodyPlate->GetComponentTransform().GetLocation(), BodyPlate->GetComponentRotation(), SpawnParams);
			if (MainBody != nullptr)
			{
				MainBody->AttachToAComponent(MySkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), FName("ArmourSocket"));
				MainBody->ToggleHighlight(false);
				MainBodyRef = MainBody;
			}
			//UE_LOG(LogTemp, Warning, TEXT("Spawned Body"));
			if (BossArmourComponent->PinClassLL != nullptr)
			{
				///UE_LOG(LogTemp, Warning, TEXT("LL Pin Class Not Null"));
				AQuackBossArmourBaseClass* PinLL = World->SpawnActor<AQuackBossArmourBaseClass>(BossArmourComponent->PinClassLL, BodyLL->GetComponentTransform().GetLocation(), BodyLL->GetComponentRotation(), SpawnParams);
				if (PinLL != nullptr)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Pin LL Spawned"));
					PinLL->AttachToActor(MainBody, FAttachmentTransformRules::KeepWorldTransform);
					PinLL->SetActorRelativeScale3D(FVector(2.0f));
					PinRefLL = PinLL;
				}
			}
			if (BossArmourComponent->PinClassLR != nullptr)
			{
				AQuackBossArmourBaseClass* PinLR = World->SpawnActor<AQuackBossArmourBaseClass>(BossArmourComponent->PinClassLR, BodyLR->GetComponentTransform().GetLocation(), BodyLR->GetComponentRotation(), SpawnParams);			
				if (PinLR != nullptr)
				{
					PinLR->AttachToActor(MainBody, FAttachmentTransformRules::KeepWorldTransform);
					PinLR->SetActorRelativeScale3D(FVector(2.0f));
					PinRefLR = PinLR;
				}
			}
			if (BossArmourComponent->PinClassUL != nullptr)
			{
				AQuackBossArmourBaseClass* PinUL = World->SpawnActor<AQuackBossArmourBaseClass>(BossArmourComponent->PinClassUL, BodyUL->GetComponentTransform().GetLocation(), BodyUL->GetComponentRotation(), SpawnParams);	
				if (PinUL != nullptr)
				{
					PinUL->AttachToActor(MainBody, FAttachmentTransformRules::KeepWorldTransform);
					PinUL->SetActorRelativeScale3D(FVector(2.0f));
					PinRefUL = PinUL;
				}
			}
			if (BossArmourComponent->PinClassUR != nullptr)
			{
				AQuackBossArmourBaseClass* PinUR = World->SpawnActor<AQuackBossArmourBaseClass>(BossArmourComponent->PinClassUR, BodyUR->GetComponentTransform().GetLocation(), BodyUR->GetComponentRotation(), SpawnParams);
				if (PinUR != nullptr)
				{
					PinUR->AttachToActor(MainBody, FAttachmentTransformRules::KeepWorldTransform);
					PinUR->SetActorRelativeScale3D(FVector(2.0f));
					PinRefUR = PinUR;
				}
			}
		}
	}

}

void AQuackBoss::StartBoss()
{
	bDontDoAnything = false;
}

// Called when the game starts or when spawned
void AQuackBoss::BeginPlay()
{
	Super::BeginPlay();

	InitialArmDamage = ArmDamage;
	InitialisePlayerCharacterReference();

	bFirstTimeHealing = true;
	bDontDoAnything = true;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FTimerHandle TempHandle;
		World->GetTimerManager().SetTimer(TempHandle, this, &AQuackBoss::StartBoss, DontDoAnythingTime, false);
		// add variable to character to prevent movement for cutscene
		// this will change to a trigger, when we get an entrance
	}
	bImmortal = true;
	MaxBossHealth = BossHealth;
	InitialFireCooldown = FireCooldown;
	CurrentBossState = BossStates::E_Idle;
	//if (MyCharacter != nullptr)
	//{
	//	MyCharacter->BossHP = MaxBossHealth / 100.0f;
	//}
	BossHealth = BossHealthRed = 10.0f;
	 
	if (MyCharacter != nullptr)
	{
		MyCharacter->BossHP = BossHealth / MaxBossHealth;
		MyCharacter->BossHPRed = BossHealth / MaxBossHealth;
		MyCharacter->bShowBossBar = false;
	}
	BossHealth = BossHealthRed = FMath::Clamp(BossHealth, 0.0f, MaxBossHealth);
	LocateNearbyPipe();
	ToggleShield(false);

	TailArrow->AttachToComponent(MySkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), TEXT("Tail6Socket"));
	//TailArrow->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));
//	TailArrow->SetRelativeLocation(FVector(0.0f, 0.0f , 50.0f));

	MouthArrow->AttachToComponent(MySkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), TEXT("MouthSocket"));
	//MouthArrow->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));
//	MouthArrow->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	BodyLL->SetRenderCustomDepth(true);
	BodyLL->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
	BodyLR->SetRenderCustomDepth(true);
	BodyLR->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
	BodyUL->SetRenderCustomDepth(true);
	BodyUL->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
	BodyUR->SetRenderCustomDepth(true);
	BodyUR->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;

	LaserParticleSystemComp->SetRelativeScale3D(FVector(0.0015));
	EMPParticleSystem->SetActive(false);

	EmissiveEyes = MySkeletalMesh->CreateDynamicMaterialInstance(2);
	GetWorld()->GetTimerManager().SetTimer(BlinkTImer, this, &AQuackBoss::CloseEyes, 0.5f, false);
}

void AQuackBoss::SetLaserSource()
{
	if (LaserParticleSystemComp != nullptr)
	{
		LaserParticleSystemComp->SetBeamSourcePoint(0, LaserParticleSystemComp->GetComponentLocation(), 0);
	}
}

void AQuackBoss::SetLaserEnd()
{
	if (LaserParticleSystemComp != nullptr)
	{
		LaserParticleSystemComp->SetBeamEndPoint(0, LaserTargetLocation);

		////// find out which way is forward
		////const FRotator Rotation = Chandelier->GetComponentRotation();
		////const FRotator YawRotation(0, Rotation.Yaw, 0);

		////// get forward vector
		////const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		////FVector EndOffset = Chandelier->GetForwardVector() + FVector(0.0f, 90.0f, 0.0f);
		////const FVector EndLocation = LaserParticleSystemComp->GetComponentLocation() + (-Direction * RaycastComponent->GetRayLength());
		////LaserParticleSystemComp->SetBeamEndPoint(0, EndLocation);

		///
		//if (MuzzleFlashParticleSystem == nullptr && FireEffect != nullptr)
		//{
		//	//UE_LOG(LogTemp, Warning, TEXT("MF Activated"));
		//	MuzzleFlashParticleSystem = UGameplayStatics::SpawnEmitterAttached(FireEffect, HarryPlasmaGun, SocketName);
		//	MuzzleFlashParticleSystem->SetWorldScale3D(FVector(0.75f));
		//}
		//if (MuzzleFlashParticleSystem != nullptr)
		//{
		//	//UE_LOG(LogTemp, Warning, TEXT("MF Location updated: %s"), *LaserTargetLocation.ToString());
		//	MuzzleFlashParticleSystem->SetWorldLocation(LaserTargetLocation);
		//}
	}
}

void AQuackBoss::EnableBeam()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enable Beam"));

	if (LaserParticleSystemComp != nullptr)
	{
		if (LaserPS != nullptr)
		{

			LaserParticleSystemComp->SetTemplate(LaserPS);
			LaserParticleSystemComp->SetVisibility(true);
		}
	}
}

void AQuackBoss::DisableBeam()
{
	//UE_LOG(LogTemp, Warning, TEXT("Disable Beam"));
	if (LaserParticleSystemComp != nullptr)
	{
		LaserParticleSystemComp->SetTemplate(EmptyPS);
		LaserParticleSystemComp->SetVisibility(false);
		//UE_LOG(LogTemp, Warning, TEXT("Disable Beam --- Set to false"));
	}
	bBossLaserAnimation = false;
}

void AQuackBoss::BeamLogic()
{
	if (RaycastComponent != nullptr && LaserParticleSystemComp != nullptr)
	{
		EnableBeam();
		// This is actually for the beam...
		// Beam Animation on
		bBossLaserAnimation = true;
		TArray<TWeakObjectPtr<AActor>> IgnoredActors;
		IgnoredActors.Add(this);
		if(RockyFloorRef != nullptr)
			IgnoredActors.Add(RockyFloorRef);
		const FRotator Rotation = Chandelier->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		FVector EndOffset = Chandelier->GetForwardVector() + FVector(0.0f, 90.0f, 0.0f);
		const FVector EndLocation = LaserParticleSystemComp->GetComponentLocation() + (-Direction * RaycastComponent->GetRayLength());
		FHitResult Hit = RaycastComponent->RaycastBossLaser(LaserParticleSystemComp, EndLocation, IgnoredActors);
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			// Anything thats not an ignored actor, will make the laser stop
			LaserTargetLocation = Hit.Location;
			//UE_LOG(LogTemp, Warning, TEXT("ActorHit: %s"), *HitActor->GetName());
			USmashByBeamComponent* FractureComp = Cast<USmashByBeamComponent>(HitActor->GetComponentByClass(USmashByBeamComponent::StaticClass()));
			if (FractureComp != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Has FractureComp"));
				FractureComp->OwnerFracture();
			}
			AQuackCharacter* Char = Cast<AQuackCharacter>(HitActor);
			if (Char != nullptr)
			{
				Char->DecreaseHealth(BeamDamage * GetWorld()->GetDeltaSeconds());
			}
		}
		else
		{
			LaserTargetLocation = EndLocation;
		}
		SetLaserSource();
		SetLaserEnd();
	}
	else
	{
		DisableBeam();
		// Beam animation off
		//bBossLaserAnimation = false;
	}
}

void AQuackBoss::SetTongueToNormal()
{
	if (MySkeletalMesh != nullptr && TongueMaterialNormal != nullptr)
	{
		if (MySkeletalMesh->GetMaterial(0) == TongueMaterialNormal) return;
		MySkeletalMesh->SetMaterial(0, TongueMaterialNormal);
	}
}

void AQuackBoss::SetTongueToHealing()
{
	if (MySkeletalMesh != nullptr && TongueMaterialHealing != nullptr)
	{
		if (MySkeletalMesh->GetMaterial(0) == TongueMaterialHealing) return;
			MySkeletalMesh->SetMaterial(0, TongueMaterialHealing);
	}
}
void AQuackBoss::ResumeFighting()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Resume fighting"));
		FTimerHandle TempTimer;
		World->GetTimerManager().SetTimer(TempTimer, this, &AQuackBoss::ChangeBackToPrevious, RecoilDuration, false);
	}
}

void AQuackBoss::ChangeBackToPrevious()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resume fighting 2"));

	if (PreviousBossState != BossStates::E_Fighting || PreviousBossState != BossStates::E_FightingTwo || PreviousBossState != BossStates::E_FightingThree || PreviousBossState != BossStates::E_FightingFour)
	{
		ChangeState(BossStates::E_FightingTwo);
		//ChangeState(PreviousBossState);
	}
	else
	{
		ChangeState(PreviousBossState);

	}
}

void AQuackBoss::CloseEyes() {
	//World->GetTimerManager().SetTimer(MeleeTimerHandle, this, &AQuackBoss::Stab, StabRate * 2, true);
	if (!bIsDead) {
		GetWorld()->GetTimerManager().SetTimer(BlinkTImer, this, &AQuackBoss::OpenEyes, 0.5f, false);
		TargetBlinkValue = 0.5f;
	}
	else TargetBlinkValue = 1.f;
}

void AQuackBoss::OpenEyes() {
	if (!bIsDead) GetWorld()->GetTimerManager().SetTimer(BlinkTImer, this, &AQuackBoss::CloseEyes, 7, false);
	TargetBlinkValue = 0;
}

// Called every frame
void AQuackBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Kris code
	if (bEyesClosed) 
	{

	}
	if (BlinkCurve != nullptr) 
	{
		CurrentBlinkValue = FMath::Lerp(CurrentBlinkValue, TargetBlinkValue, DeltaTime);
		EmissiveEyes->SetScalarParameterValue(FName("EyesGlowMultiplier"), BlinkCurve->GetFloatValue(CurrentBlinkValue));
	}

	if (bDontDoAnything || bIsDead) return;
	MapBossMovementToPlayer(DeltaTime);
	DrainInTick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("DrainRedBar: %s"), bBeginDrainingRedBar ? TEXT("TRUE") : TEXT("FALSE")));
	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("CurrentDrainCache: %f"), CurrentDrainCache));
	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("HP: %f, RedHP: %f"), BossHealth,BossHealthRed));

	if (PinRefLL != nullptr && PinRefUL != nullptr && PinRefLR != nullptr && PinRefUR != nullptr)
	{
		if (PinRefLL->bHasBeenDestroyed && PinRefLR->bHasBeenDestroyed && PinRefUL->bHasBeenDestroyed && PinRefUR->bHasBeenDestroyed)
		{
			if (MainBodyRef != nullptr && bImmortal)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Fracture"));
				// TEMP FIX
				// Hide Cannon, whilst armour is on, so it doesnt peak through
				// Due to wierd socket not following bug
				bImmortal = false;
				LaserCannon->SetHiddenInGame(false);
				MainBodyRef->Fracture();
				// Should auto push into next phase instead of having to damage him after killing 4 pins
				ShouldEnterHealingPhase();
				if (MyCharacter != nullptr)
				{
					MyCharacter->bShowBossBar = true;
				}
			}
		}
		else
		{
			bImmortal = true;
		}
	}
	if (BossHealth >= MaxBossHealth)
	{
		bFirstTimeHealing = false;
		if (TargettedPipe != nullptr)
		{
			TargettedPipe->SetDescend(false);
			TargettedPipe->ToggleHighlight(false);
		}
		// THIS IS SOMEWHAT OF AN EXPERIEMENT ... PLIS WORK -- RECOILD WHEN FULLY HEALED
		//CurrentAnimationState = AnimationStates::E_AnimRecoil;		
		if (CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingTwo 
			|| CurrentBossState == BossStates::E_HealingThree || CurrentBossState == BossStates::E_HealingFour || CurrentBossState == BossStates::E_Poisoned || CurrentBossState == BossStates::E_Paused)
		{
			ChangeState(BossStates::E_Recoiling);
			ResumeFighting();
		}
		else
		{
			ChangeState(BossStates::E_Recoiling);
			ResumeFighting();

			//ChangeState(BossStates::E_Idle);
			//UE_LOG(LogTemp, Warning, TEXT("FullyHealed: Else ran"));
		}
		BossHealth -= 1.0f;
	}
	//LocateNearbyPipe();
	HandleStates(DeltaTime);
}

void AQuackBoss::ToggleShield(bool Visiblity)
{
	if (Shield != nullptr)
	{
		Shield->SetVisibility(Visiblity);
		bShieldUp = Visiblity;
	}
}

void AQuackBoss::BeginAscend()
{
	//bAscend = true;
}

void AQuackBoss::Descend(float DeltaTime)
{

}

void AQuackBoss::Ascend(float DeltaTime)
{

}
void AQuackBoss::HandleStates(float DeltaTime)
{
	switch (CurrentBossState)
	{
		case BossStates::E_Idle:
		{
			SetTongueToNormal();
			CurrentAnimationState = AnimationStates::E_AnimIdle;
			// Below 80% phase one healing
			if (LowerPipes.Num() + UpperPipes.Num() + 1 == 4 && TargettedPipe != nullptr)
			{
				ChangeState(BossStates::E_Fighting);
			}
			// Below 60% Phase two healing
			else if (LowerPipes.Num() + UpperPipes.Num() + 1 == 3 && TargettedPipe != nullptr)
			{
				ChangeState(BossStates::E_FightingTwo);
			}
			// Below 40% Phase three healing
			else if (LowerPipes.Num() + UpperPipes.Num() + 1 == 2 && TargettedPipe != nullptr)
			{
				ChangeState(BossStates::E_FightingThree);
			}
			// Below 20% Phase four healing
			else if (LowerPipes.Num() + UpperPipes.Num() + 1 == 1 && TargettedPipe != nullptr)
			{
				ChangeState(BossStates::E_FightingFour);
			}
			else
			{
				ChangeState(BossStates::E_FightingFour);
			}
			//ToggleShield(false);
			//RotateTowardsPlayer();
			//ShootFromTail(DeltaTime);
			break;
		}
		case BossStates::E_HealingOne:
		{
			SetTongueToHealing();
			RotateTowardsPipe();
			ToggleShield(true);
			if (bFacingTargettedPipe )//&& !bMeleeStabbing)
			{
				CurrentAnimationState = AnimationStates::E_AnimLatch;
				BeginPipeDrain();
				Regenerate(DeltaTime, bFirstTimeHealing);
			}
 			else if (bFacingTargettedPipeLower)// && !bMeleeStabbing)
			{
				CurrentAnimationState = AnimationStates::E_AnimLatchLower;
				BeginPipeDrain();
				Regenerate(DeltaTime, bFirstTimeHealing);
			}
			break;
		}
		case BossStates::E_HealingTwo:
		{
			SetTongueToHealing();
			RotateTowardsPipe();
			ToggleShield(true);
			//if (!bFacingTargettedPipe) return;
			//CurrentAnimationState = AnimationStates::E_AnimLatch;
			//BeginPipeDrain();
			//Regenerate(DeltaTime);
			if (bFacingTargettedPipe)
			{
				CurrentAnimationState = AnimationStates::E_AnimLatch;
				BeginPipeDrain();
				Regenerate(DeltaTime);
			}
			else if (bFacingTargettedPipeLower)
			{
				CurrentAnimationState = AnimationStates::E_AnimLatchLower;
				BeginPipeDrain();
				Regenerate(DeltaTime);
			}
			break;
		}
		case BossStates::E_HealingThree:
		{
			SetTongueToHealing();
			RotateTowardsPipe();
			ToggleShield(true);
			if (bFacingTargettedPipe)
			{
				CurrentAnimationState = AnimationStates::E_AnimLatch;
				BeginPipeDrain();
				Regenerate(DeltaTime);
			}
			else if (bFacingTargettedPipeLower)
			{
				CurrentAnimationState = AnimationStates::E_AnimLatchLower;
				BeginPipeDrain();
				Regenerate(DeltaTime);
			}
			break;
		}
		case BossStates::E_HealingFour:
		{
			SetTongueToHealing();
			RotateTowardsPipe();
			ToggleShield(true);
			if (bFacingTargettedPipe)
			{
				CurrentAnimationState = AnimationStates::E_AnimLatch;
				BeginPipeDrain();
				Regenerate(DeltaTime);
			}
			else if (bFacingTargettedPipeLower)
			{
				CurrentAnimationState = AnimationStates::E_AnimLatchLower;
				BeginPipeDrain();
				Regenerate(DeltaTime);
			}
			break;
		}
		case BossStates::E_Poisoned:
		{
			SetTongueToNormal();
			StopFacingPipe();
			RotateTowardsPipe();
			CheckForPoisoned(DeltaTime);
			//UE_LOG(LogTemp, Warning, TEXT("Poisonned"));
			ToggleShield(false);
			EndPipeDrain();
			break;
		}
		case BossStates::E_Recoiling:
		{
			SetTongueToNormal();
			if (TargettedPipe != nullptr)
			{
				if (TargettedPipe->bLowerPipe)
				{
					CurrentAnimationState = AnimationStates::E_AnimRecoilLower;
				}
				else
				{
					CurrentAnimationState = AnimationStates::E_AnimRecoil;
				}
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("TargettedPipe is nullptr in E_Recoiling"));
			}
			StopFacingPipe();
			// Ors might need to be ands not sure
			if (!bFacingTargettedPipe && !bFacingTargettedPipeLower)
			{
				RotateTowardsPlayer();
				ToggleShield(false);
				EndPipeDrain();
			}
			break;
		}
		case BossStates::E_Fighting:
		{
			SetTongueToNormal();
			// MIGHT NEED THIS, SINCE RESUME FIGHTING, MIGHT NOT CHECK FOR THIS BOOL
			if (bFacingTargettedPipe && bFacingTargettedPipeLower) return;

			//ShootFromTail(DeltaTime);
			ToggleShield(false);
			RotateTowardsPlayer();

			InitiateMeleeAttacks();
			if (!CheckForMeleeAttack())
			{
				//StartTailShot();
				//float NewFireRate = BossAttacksComponent->GetBileFireRate() / 4;
				//StartBileShot(NewFireRate);
			}
			//StartTailShot();
			//ShootFromTail(DeltaTime);

			break;
		}
		case BossStates::E_FightingTwo:
		{
			BeamLogic();
			SetTongueToNormal();
			if (bFacingTargettedPipe && bFacingTargettedPipeLower) return;
			ToggleShield(false);
			RotateTowardsPlayer();
			if (!CheckForMeleeAttack())
			{
				//BeginMultipleAttacksPattern();
				//float NewFireRate = BossAttacksComponent->GetBileFireRate() / 4;
				//StartBileShot(NewFireRate);
			}
			break;
		}
		case BossStates::E_FightingThree:
		{
			SetTongueToNormal();
			if (bFacingTargettedPipe && bFacingTargettedPipeLower) return;

			//UE_LOG(LogTemp, Warning, TEXT("Fighting Three : Check for Melee Attack : %s"), Check ? TEXT("true") : TEXT("false"));
			if (/*!CheckForMeleeAttack() &&*/ BossAttacksComponent != nullptr)
			{
				// Bile during spawn phase, but half as often - balance
				float NewFireRate = BossAttacksComponent->GetBileFireRate() / 2;
				StartBileShot(NewFireRate);
			}
			// Turns bIsSpawning to true // 30s it turns off
			if(!bFightingThreeDrop)
				BeginWaveSpawningCycle();
			bFightingThreeDrop = true;
			// Make sure chandelier logic is only active
			// When minions are afoot
			if (MinionFactory != nullptr)
			{
				switch (MinionFactory->AreMinionsAlive())
				{
					case true:
					{
						if (bIsSpawning)
						{
							if (ChandelierDropComponent != nullptr)
							{
								ChandelierDropComponent->FinishAdjust();
								ChandelierDropComponent->Lower();
							}
							ToggleShield(true);
						}
						else
						{
							if (ChandelierDropComponent != nullptr)
							{
								ChandelierDropComponent->FinishAdjust();
								ChandelierDropComponent->Raise();
							}
							RotateTowardsPlayer();
							ToggleShield(false);
						}
						break;
					}
					case false:
					{
						if (ChandelierDropComponent != nullptr)
						{
							ChandelierDropComponent->FinishAdjust();
							ChandelierDropComponent->Raise();
						}
						RotateTowardsPlayer();
						ToggleShield(false);
						break;
					}
				}
			}
			break;
		}
		case BossStates::E_FightingFour:
		{
			SetTongueToNormal();
			if (bFacingTargettedPipe && bFacingTargettedPipeLower) return;

			//if (!CheckForMeleeAttack())
			//{
				BeginMultipleAttacksPattern();
			//}
			// Only Drop Once during the phases - and get back up regardless after 30s
			if(!bFightingFourDrop)
				BeginWaveSpawningCycle();
			bFightingFourDrop = true;
			if (MinionFactory != nullptr)
			{
				switch (MinionFactory->AreMinionsAlive())
				{
					case true:
					{
						if (bIsSpawning)
						{
							if (ChandelierDropComponent != nullptr)
							{
								ChandelierDropComponent->FinishAdjust();
								ChandelierDropComponent->Lower();
							}
							ToggleShield(true);
						}
						else
						{
							if (ChandelierDropComponent != nullptr)
							{
								ChandelierDropComponent->FinishAdjust();
								ChandelierDropComponent->Raise();
							}
							RotateTowardsPlayer();
							ToggleShield(false);
						}
						break;
					}
					case false:
					{
						if (ChandelierDropComponent != nullptr)
						{
							ChandelierDropComponent->FinishAdjust();
							ChandelierDropComponent->Raise();
						}
						RotateTowardsPlayer();
						ToggleShield(false);
						break;
					}
				}
			}
			break;
		}
		case BossStates::E_Paused:
		{
			break;
		}
	}
}

void AQuackBoss::InitiateMeleeAttacks()
{
	if (bMeleeStabbing) return;
	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	if (AnimationComponent != nullptr)
	{
		UAnimMontage* AnimMeleeLeft = AnimationComponent->GetMeleeAnimLeft();
		UAnimMontage* AnimMeleeRight = AnimationComponent->GetMeleeAnimRight();
		UAnimMontage* AnimMeleeBoth = AnimationComponent->GetMeleeAnimBoth();

		if (AnimMeleeLeft != nullptr && AnimMeleeRight != nullptr && AnimMeleeBoth != nullptr)
		{
			float MeleeLeft = AnimMeleeLeft->GetPlayLength();
			float MeleeRight = AnimMeleeRight->GetPlayLength();
			float MeleeBoth = AnimMeleeBoth->GetPlayLength();
			float StabRate = FMath::Max3(MeleeLeft, MeleeRight, MeleeBoth);
			//float StabRate = AnimInst->Montage_Play(AnimationComponent->GetMeleeAnimLeft());
			World->GetTimerManager().SetTimer(MeleeTimerHandle, this, &AQuackBoss::Stab, StabRate * 2, true);
			World->GetTimerManager().SetTimer(CharHitMeleeTimerHandle, this, &AQuackBoss::EnableMelee, StabRate * 2, true);
			bMeleeStabbing = true;
		}
	}
}

void AQuackBoss::EnableMelee()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	// OKAY, SO THIS BOOL IS PURELY FOR PREVENTING INSTA KILLS
	bMeleeAttacking = false;
	// Keep turning it off at the end of each attack swing, 
	// but it gets reset at the start of the next one
}

void AQuackBoss::Stab()
{
	// This bool allows damage to be dealt in moderation
	// See trigger enter
	// Adjusted
	//bMeleeAttacking = true;
	UAnimInstance* AnimInst = MySkeletalMesh->GetAnimInstance();
	if (AnimInst == nullptr || AnimationComponent == nullptr) return;
	// Switch statement is extensible, in case we wanted more variations on the attack
	// Just increase MaxMeleeVariations, And add appropriately
	switch (MeleeCounter)
	{
		case 1:
		{
			if (AnimationComponent->GetMeleeAnimLeft() != nullptr)
			{
				ArmDamage = InitialArmDamage;
				AnimInst->Montage_Play(AnimationComponent->GetMeleeAnimLeft(), 1.0f);
			}
			break;
		}
		case 2:
		{
			if (AnimationComponent->GetMeleeAnimBoth() != nullptr)
			{
				ArmDamage = (InitialArmDamage * 2);
				AnimInst->Montage_Play(AnimationComponent->GetMeleeAnimBoth(), 1.0f);
			}
			break;
		}
		case 3:
		{
			if (AnimationComponent->GetMeleeAnimRight() != nullptr)
			{
				ArmDamage = InitialArmDamage;
				AnimInst->Montage_Play(AnimationComponent->GetMeleeAnimRight(), 1.0f);
			}
			break;
		}
	}
	bIsStabbingAnimation = true;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FTimerHandle StabOffHandle;
		World->GetTimerManager().SetTimer(StabOffHandle, this, &AQuackBoss::StabPause, 2.3f, false);
	}
	MeleeCounter++;
	if (MeleeCounter > MaxMeleeVariations)
	{
		MeleeCounter = 1;
	}
}

void AQuackBoss::StabPause()
{
	bIsStabbingAnimation = false;
}

void AQuackBoss::ClearStabRoutine()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().ClearTimer(MeleeTimerHandle);
	World->GetTimerManager().ClearTimer(CharHitMeleeTimerHandle);
	bMeleeAttacking = false;
	bMeleeStabbing = false;
}

void AQuackBoss::StopFacingPipe()
{
	if (bFacingTargettedPipe || bFacingTargettedPipeLower)
	{
		UWorld* const World = GetWorld();
		if (World == nullptr) return;
		FTimerHandle PipeHandle;
		// This is the duration of recoil animation
		World->GetTimerManager().SetTimer(PipeHandle, this, &AQuackBoss::SetFacingPipeOff, 2.0f, false);
	}
}

void AQuackBoss::SetFacingPipeOff()
{
	// Allow boss to horizontally strafe again after recoiling
	bStopChase = false;
	// Boss is no longer facing pipe, after recoil, so allow rotation
	bFacingTargettedPipe = false;
	bFacingTargettedPipeLower = false;
	//// Turn off overriden height controls
	//if(ChandelierDropComponent != nullptr)
	//	ChandelierDropComponent->FinishAdjust();
}

void AQuackBoss::ResetMelee()
{
	//UE_LOG(LogTemp, Warning, TEXT("Melee Reset From bMeleeStarted timer"));
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().ClearTimer(MeleeCheckTimerHandle);
	bMeleeStarted = false;
	//ResumeFighting();
}

void AQuackBoss::MapBossMovementToPlayer(float DeltaTime)
{
	if (MyCharacter != nullptr)
	{
		Positions = FMyMap(GetActorLocation(), MyCharacter->GetActorLocation());
		//if (Positions.PlayerPosition.X > Positions.BossPosition.X)
		//{
		// When healing stop chase is true
		if (bStopChase)
		{
			if (ChandelierDropComponent != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("chandelierDropComp not null"));
				switch (CurrentTargettedPipeTransform.bIsLowerPipe)
				{
					case true:
					{
						//UE_LOG(LogTemp, Warning, TEXT("StopChase True - Adjust to Lower"));
						ChandelierDropComponent->AdjustToLowerPipeHeight();
						break;
					}
					case false:
					{
						//UE_LOG(LogTemp, Warning, TEXT("StopChase True - Adjust to upper"));
						ChandelierDropComponent->AdjustToUpperPipeHeight();
						break;
					}
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("StopChase True - Moving to centre"));
			//UE_LOG(LogTemp, Warning, TEXT("IsLower : %s"), CurrentTargettedPipeTransform.bIsLowerPipe ? TEXT("True its lower") : TEXT("false its upper"));
			float BossLoc = GetActorLocation().X;
			BossLoc = FMath::FInterpConstantTo(BossLoc, CentrePosition, DeltaTime, BossStafeSpeed);
			// Should manipulate height of boss during all stages to match player
			// See what this is like, if not good enough, make separate version only for lasering
			float BossLocZ = GetActorLocation().Z;

			SetActorLocation(FVector(BossLoc, GetActorLocation().Y, BossLocZ));
		}
		else
		{

			float BossLoc = GetActorLocation().X;
			float TargetLoc = Positions.PlayerPosition.X;
			float BossLocZ = GetActorLocation().Z;
			TargetLoc = FMath::Clamp(TargetLoc, MinBossX, MaxBossX);
			if (CurrentBossState == BossStates::E_FightingTwo)
			{
				if (ChandelierDropComponent != nullptr)
				{
					//UE_LOG(LogTemp, Warning, TEXT("PlayerPos: %f"), Positions.PlayerPosition.Z);

					if (Positions.PlayerPosition.Z < 0.0f)
					{
						//BossLocZ = FMath::FInterpConstantTo(BossLocZ, ChandelierDropComponent->GetGroundLaserHeight(), DeltaTime, BossStafeSpeed);
						//UE_LOG(LogTemp, Warning, TEXT("BossLocZ Down: %f"), BossLocZ);
						ChandelierDropComponent->AdjustToLowerPipeHeight();

					}
					else
					{
						//BossLocZ = FMath::FInterpConstantTo(BossLocZ, ChandelierDropComponent->GetStartingLaserHeight(), DeltaTime, BossStafeSpeed);
						//UE_LOG(LogTemp, Warning, TEXT("BossLocZ Up: %f"), BossLocZ);
						ChandelierDropComponent->AdjustToUpperPipeHeight(true);
					}
				}
			}
			else
			{
				if (ChandelierDropComponent != nullptr)
				{
					ChandelierDropComponent->FinishAdjust();
					//UE_LOG(LogTemp, Warning, TEXT("FinishAdjust Trolling me: %f"), BossLocZ);

				}
			}
			BossLoc = FMath::FInterpConstantTo(BossLoc, TargetLoc, DeltaTime, BossStafeSpeed);
			//UE_LOG(LogTemp, Warning, TEXT("SetActorLocation: %f"), BossLocZ);
			SetActorLocation(FVector(BossLoc, GetActorLocation().Y, BossLocZ));
		}

		//}
		//else if (Positions.PlayerPosition.X <= Positions.BossPosition.X)
		//{
		//	float BossLoc = GetActorLocation().X;
		//	BossLoc = FMath::FInterpConstantTo(BossLoc, Positions.PlayerPosition.X, DeltaTime, BossStafeSpeed);
		//	SetActorLocation(FVector(BossLoc, GetActorLocation().Y, GetActorLocation().Z));
		//}
	}
}

// DEPRECATED
bool AQuackBoss::CheckForMeleeAttack()
{
	// THINK I MAY OF FIGURED IT OUT
	// COZ WE STOP ALL ATTACKS FOR MELEE
	// HE DOESNT KNOW TO STOP MELEEING UNTIL
	// THE ATTACK TIMER GOES THROUGH
	if (MyCharacter == nullptr) return false;
	//if (MinionFactory != nullptr)
	//{
	//	if (MinionFactory->AreMinionsAlive() && !bIsSpawning) return false;
	//	if (!MinionFactory->AreMinionsAlive() && !bIsSpawning) return false;
	//}
	// THIS TEMPORARILY FIXES THE WIERD MELEE BUG - when in a spawning phase - when he lowers he melees 3 times for some reason
	//if (bIsSpawning) return false;// && MinionFactory->AreMinionsAlive()) return false;

	FVector BossLocation = GetActorLocation();
	FVector CharLocation = MyCharacter->GetActorLocation();
	float Distance = FVector::Dist(BossLocation, CharLocation);
	
	UWorld* const World = GetWorld();
	if (FMath::Abs(Distance) <= MeleeRangeDistanceThreshold)
	{
		if (!bMeleeStarted)
		{
			if (World != nullptr)
			{
				World->GetTimerManager().SetTimer(MeleeCheckTimerHandle, this, &AQuackBoss::ResetMelee, 1.75f, false);

				bMeleeStarted = true;
				//UE_LOG(LogTemp, Warning, TEXT("Dist: %f , Threshold: %f"), Distance, MeleeRangeDistanceThreshold);
				// Basically, cancel everything boss was doing, to launch melee attacks
				// Might wanna implement a buffer, so player cant abuse this, by standing melee range
				// or alternatively, make his punches REALLY powerful xD
				// Note:: New Punch anim, for alternating shots would be nice

				// THERE IS A SMALL BUG, THAT IF HE WAS IN BILE SHOT
				// AND YOU TRIGGER MELEE ATTACK
				// IT WILL INSTA RESET TO TAIL
				// SO MIGHT NEED TO CACHE WHAT PART OF MULTIPLE PHASE YOU WAS IN, TO RESUME IT
				StopTailShot();
				StopBileShot();
				StopBileSpray();
				ResetMultipleAttacksPattern();
				CurrentAnimationState = AnimationStates::E_AnimMelee;

				//if(bIsSpawning)
				// TESTING DO WE EVEN NEED THIS
				//ClearWaveSpawningTimer();
				//if(MeleeCheckTimerHandle.IsValid())
				// K WTH, ISTIMERACTIVE FUNCTION ALWAYS RETURNS FALSE GG
				///bool TimerActive = World->GetTimerManager().IsTimerActive(MeleeCheckTimerHandle);



				//if(!bMeleeTimerTicking)
				//{
				//	bMeleeTimerTicking = true;
				//	World->GetTimerManager().SetTimer(MeleeCheckTimerHandle, this, &AQuackBoss::EnableMelee, 1.75f, false);
				//	UE_LOG(LogTemp, Warning, TEXT("Timer to reset melee activated : should not see this for another 1.75 seconds, Bool Timerticking: %s"), bMeleeTimerTicking ? TEXT("true") : TEXT("false"));
				//}
			}
		}
	}
	else
	{
		//bMeleeStarted = false;
		//ResetMelee();
		// If melee is not in motion, set to deactivate, and you run outside the melee range check
		// just turn it off
		if (!bMeleeTimerTicking)
		{
			//bMeleeStarted = false;
		}
	}
	//else if (bMeleeAttacking)
	//{
	//	// THIS MIGHT FIX MELEE BUG
	//	return true;
	//}
	//UE_LOG(LogTemp, Warning, TEXT("Dist: %f , Threshold: %f"), Distance, MeleeRangeDistanceThreshold);

	return bMeleeStarted;
}

void AQuackBoss::BeginWaveSpawningCycle()
{
	if (bIsSpawning) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BeginWaveSpawning : Phase Fighting Three"));
	//	UE_LOG(LogTemp, Warning, TEXT("BeginSpawningCycle"));
		//ChangeAttack();
		SpawnMinions();
		FTimerHandle EndOfWaveHandle; // WaveSpawnerHandle Deprecated
		World->GetTimerManager().SetTimer(EndOfWaveHandle, this, &AQuackBoss::MinionExpiredRaise, MinionExpiringDelay, false);
		bIsSpawning = true;
	}
}

void AQuackBoss::MinionExpiredRaise()
{
	bIsSpawning = false;
}

void AQuackBoss::SpawnMinions()
{
	if (MinionFactory != nullptr)
	{
		// Change state, cleanses the timer used to routinely spawn these,
		// It was possible to initiate that function several times in 
		// A short period of time, due to the recoil patch
		// This was inadvertedly, making the boss spawn, TONS
		// of minions, so this check should prevent you getting swarmed
		/*if (MinionFactory->AreMinionsAlive())
		{
			return;
		}*/
		if (MinionWaves.Num() == 0) return;
		if (WaveSpawnCounter > MinionWaves.Num() - 1) return;
		MinionFactory->SpawnMinionWave(MinionWaves[WaveSpawnCounter]);
		WaveSpawnCounter++;
		/*if (WaveSpawnCounter % 2 == 0)
		{
			MinionFactory->SpawnMinionWaveA();
		}
		else
		{
			MinionFactory->SpawnMinionWaveB();
		}*/
	}
}

void AQuackBoss::ClearWaveSpawningTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("Wave Spawn Timer RESET : Boss"));
	//bIsSpawning = false;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(WaveSpawnerTimer);
	}
}

void AQuackBoss::BeginMultipleAttacksPattern()
{
	if (bUsingPrimaryAttack) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BeginMultipleAttackPatterns"));
		//ChangeAttack();
		ChangeAttack();
		World->GetTimerManager().SetTimer(AttackIterationTimer, this, &AQuackBoss::ChangeAttack, 1.0f / ChangeAttackPatternRate, true);
		// Bool to make this function in update, run once
		// As it acts as an initiator for the repeated timer
		bUsingPrimaryAttack = true;
	}
}

void AQuackBoss::ChangeAttack()
{
	// THINK WE NEED AN ENUM TO TRACK ATTACK TYPES

	//// If you are tail shooting , and you wasnt previously using bile
	//// Switch to bile
	//if (bIsTailShooting)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("BeginMultipleAttackPatterns - Attack change to bile"));
	//	// bTailShooting to false, and clears tail timer
	//	StopTailShot();
	//	// set bilespitting to true once then repeat the attack function
	//	StartBileShot();
	//}

	//else
	//{
	//	//if (bWasTailMostRecent)
	//	//{
	//	//	StopBileShot();
	//	//	StartTailShot();
	//	//}
	//	//else if (bWasBileMostRecent)
	//	//{
	//	//	StopTailShot();
	//	//	StartBileShot();
	//	//}
	//	
	//	//UE_LOG(LogTemp, Warning, TEXT("BeginMultipleAttackPatterns - Attack change to tail"));
	//	StopBileShot();
	//	StartTailShot();
	//}

	// Start on bile spit

	// THIS WILL CHANGE AT SOME POINT
	// WHEN NEW ATTACKS ARE READY
	if (BossAttacksComponent != nullptr)
	{
		if (BossAttacksComponent->GetIsBileSpitting())
		{
			StopBileShot();
			StopTailShot();
			//StartTailShot();
			StartBileSpray();
		}
		// Tail Attack has been made redundant
		else if(BossAttacksComponent->GetIsTailShooting())
		{
			StopTailShot();
			StopBileSpray();
			// set bilespitting to true once then repeat the attack function
			StartBileShot();
		}
		else if (BossAttacksComponent->GetIsBileSpraying())
		{
			StopBileSpray();
			StopTailShot();
			StartBileShot();
		}
		else
		{
			// This case is coz when he resets his phase, both attacks are false so, in that event, choose one randomly
			int Rand = FMath::RandRange(0, 3);
			if (Rand == 0)
			{
				StopTailShot();
				StopBileSpray();
				StartBileShot();
			}
			else
			{
				StartBileShot();
				StopBileSpray();
				StopTailShot();
			}
		}
	}
}

void AQuackBoss::ResetMultipleAttacksPattern()
{
	bUsingPrimaryAttack = false;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BeginMultipleAttackPatterns - RESET"));
		World->GetTimerManager().ClearTimer(AttackIterationTimer);
	}
}

void AQuackBoss::ShouldEnterHealingPhase()
{
	if (CurrentBossState == BossStates::E_Poisoned) return;
	if (LowerPipes.Num() == 0 && UpperPipes.Num() == 0 && TargettedPipe == nullptr)
	{
		//ChangeState(BossStates::E_Idle);
		return;
	}
	else
	{
		// Below 80% phase one healing
		// + 1 is since, when a pipe is selected it is removed from the array
		//if (BossHealth <= ((4 * MaxBossHealth) / 5) && LowerPipes.Num() + UpperPipes.Num() + 1 == 4 && TargettedPipe != nullptr)
		//{
		if (!bImmortal && LowerPipes.Num() + UpperPipes.Num() + 1 == 4 && TargettedPipe != nullptr && BossHealth<=(4*MaxBossHealth/5))
		{	
			ChangeState(BossStates::E_HealingOne);
			if (MyCharacter != nullptr)
			{
				MyCharacter->ForceNeedleGun();
				EMPParticleSystem->SetActive(true);
				UE_LOG(LogTemp, Warning, TEXT("Should Enter Healing: + ForceNeedleSwap"));
			}
		}
		// Below 60% Phase two healing
		else if (BossHealth <= ((3 * MaxBossHealth) / 5) && LowerPipes.Num() + UpperPipes.Num() + 1 == 3 && TargettedPipe != nullptr)
		{
			ChangeState(BossStates::E_HealingTwo);
			if (MyCharacter != nullptr)
			{
				MyCharacter->ForceNeedleGun();
				EMPParticleSystem->SetActive(true);
				UE_LOG(LogTemp, Warning, TEXT("Should Enter Healing: + ForceNeedleSwap"));
			}
		}
		// Below 40% Phase three healing
		else if (BossHealth <= (2 * MaxBossHealth / 5) && LowerPipes.Num() + UpperPipes.Num() + 1 == 2 && TargettedPipe != nullptr)
		{
			ChangeState(BossStates::E_HealingThree);
			if (MyCharacter != nullptr)
			{
				MyCharacter->ForceNeedleGun();
				EMPParticleSystem->SetActive(true);
				UE_LOG(LogTemp, Warning, TEXT("Should Enter Healing: + ForceNeedleSwap"));
			}
		}
		// Below 20% Phase four healing
		else if (BossHealth <= (MaxBossHealth / 5) && LowerPipes.Num() + UpperPipes.Num() + 1 == 1 && TargettedPipe != nullptr)
		{
			ChangeState(BossStates::E_HealingFour);
			if (MyCharacter != nullptr)
			{
				MyCharacter->ForceNeedleGun();
				EMPParticleSystem->SetActive(true);
				UE_LOG(LogTemp, Warning, TEXT("Should Enter Healing: + ForceNeedleSwap"));
			}
		}
		
		//AQuackCharacter* TempCharacter = Cast<AQuackCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());

	}
}

void AQuackBoss::InitialisePlayerCharacterReference()
{
	// Iterate the world for the existing player
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AQuackCharacter> ActorItr(World); ActorItr; ++ActorItr)
		{
			MyCharacter = *ActorItr;
		}
	}
}

void AQuackBoss::SufferDamage(float Amount)
{
	// IMMORTAL IS FOR ARMOUR ONLY
	if (bImmortal) return;

	if (CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingTwo || CurrentBossState == BossStates::E_HealingThree || CurrentBossState == BossStates::E_HealingFour) return;
	if (bShieldUp) return;
	// Utility Function to Deduct Damage from Health then clamp for safety
	BossHealth -= Amount;
	if (MyCharacter != nullptr)
	{
		MyCharacter->BossHP = BossHealth / MaxBossHealth;
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (!World->GetTimerManager().IsTimerActive(RedBarHandle))
			{
				World->GetTimerManager().SetTimer(RedBarHandle, this, &AQuackBoss::LowerRedHealthBar, RedDelay, false);
			}
		}
	}
		
	BossHealth = FMath::Clamp(BossHealth, 0.0f, MaxBossHealth);
	BossHealthRed = FMath::Clamp(BossHealthRed, 0.0f, MaxBossHealth);

	CheckForDead();
}

void AQuackBoss::LowerRedHealthBar()
{
	bBeginDrainingRedBar = true;
	CurrentDrainCache = BossHealth;
}

void AQuackBoss::DrainInTick(float DeltaTime)
{
	if (bBeginDrainingRedBar)
	{
		if (MyCharacter != nullptr)
		{
			BossHealthRed -= (DrainRate * DeltaTime);
			MyCharacter->BossHPRed = BossHealthRed / MaxBossHealth;
			//UE_LOG(LogTemp, Warning, TEXT("BossRedHP: %f, Threshold: %f"), MyCharacter->BossHPRed, CurrentDrainCache);
			if (BossHealthRed <= CurrentDrainCache)
			{
				RenewRedBarHandle();
			}
				//BossHealth / MaxBossHealth;
			//FTimerHandle TempHandle;
			//UWorld* const World = GetWorld();
			//if (World != nullptr)
			//{
			//	World->GetTimerManager().SetTimer(TempHandle, this, &AQuackBoss::LowerRedHealthBar, RedDelay, false);
			//}
		}
	}
}

void AQuackBoss::RenewRedBarHandle()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(RedBarHandle);
		bBeginDrainingRedBar = false;	
		// Check for More Damage To Reduce
		if (!World->GetTimerManager().IsTimerActive(RedBarHandle))
		{
			World->GetTimerManager().SetTimer(RedBarHandle, this, &AQuackBoss::LowerRedHealthBar, RedDelay, false);
		}
		
	}
}

void AQuackBoss::Regenerate(float DeltaTime, bool bOverride)
{
	// Used when boss enters regen phase ---- Poison Mechanic
	//if (!bIsRegenerating) return;
	//if (bPoisonned) return;
	// 15hp/s / 640hp
	if (bOverride)
	{
		// Want a 5s matinee now as well
		BossHealth += DeltaTime * BossRegenRate * 15.0f;

		if (MyCharacter != nullptr)
		{
			MyCharacter->BossHP = BossHealth / MaxBossHealth;
			MyCharacter->SetPlayerMovement(true);
			if (CutsceneContainer != nullptr && !bPlayOnce)
			{
				bPlayOnce = true;
				switch (CurrentTargettedPipeTransform.bIsLeftPipe)
				{
					case true:
					{
						bool bSuccess = CutsceneContainer->PlayLowerPipeLeftMatinee();
						break;
					}
					case false:
					{
						bool bSuccess = CutsceneContainer->PlayLowerPipeRightMatinee();
						break;
					}
				}
				//float Duration = CutsceneContainer->GetMatineeLength(CutsceneContainer->GetLowerPipeMatinee());
			}
			FTimerHandle TempTimer;
			UWorld* TempWorld = GetWorld();
			if (TempWorld != nullptr)
			{
				// Technically some overhead here, but not noticeable since this function is just a flag setter
				TempWorld->GetTimerManager().SetTimer(TempTimer, this, &AQuackBoss::RestorePlayerMovement, 5.0f, false);
			}
			if (BossHealth > BossHealthRed)
			{
				BossHealthRed = BossHealth;
				MyCharacter->BossHPRed = BossHealthRed / MaxBossHealth;
			}
			RenewRedBarHandle();
		}
	}
	else
	{
		BossHealth += BossRegenRate * DeltaTime;

		if (MyCharacter != nullptr)
		{
			MyCharacter->BossHP = BossHealth / MaxBossHealth;
			if (BossHealth > BossHealthRed)
			{
				BossHealthRed = BossHealth;
				MyCharacter->BossHPRed = BossHealthRed / MaxBossHealth;
			}
			RenewRedBarHandle();
		}
	}
}

void AQuackBoss::RestorePlayerMovement()
{
	if (MyCharacter != nullptr)
	{
		MyCharacter->SetPlayerMovement(false);
	}
}

void AQuackBoss::CheckForDead()
{
	// Check Pointer to this is valid before terminating
	check(this);
	// If Boss is primed and ready to die - proceed
	if (BossHealth <= 0.0f)
	{
		if (AudioManager != nullptr)
		{
			AudioManager->ChangeStateTo(AudioStates::E_Victory, 0.25f, 0.5f);
		}
		// Play Death matinee / animation / spawn effects etc etc
		// Quick fix to kill bar when boss is dead
		BossHealthRed = 0.0f;
		if(MyCharacter != nullptr)
			MyCharacter->bShowBossBar = false;

		UWorld* TempWorld = GetWorld();
		if (TempWorld != nullptr)
		{
			AQuackGameMode* TempGameMode = Cast<AQuackGameMode>(TempWorld->GetAuthGameMode());
			if (TempGameMode != nullptr)
			{
				float RealtimeSeconds = UGameplayStatics::GetRealTimeSeconds(TempWorld);
				FString TempString = TEXT("Boss Killed At: ");
				TempString += FString::FromInt(RealtimeSeconds);
				TempString += LINE_TERMINATOR;
				TempGameMode->AddToString(TempString);
				TempGameMode->WriteToFile();
			}
		}
		BossDeath();
	}
}

void AQuackBoss::BossDeath() {
	bIsDead = true;
	GetWorld()->GetTimerManager().SetTimer(BlinkTImer, this, &AQuackBoss::CloseEyes, 3.f, false);
}

void AQuackBoss::CheckForPoisoned(float DeltaTime)
{
	if (MyCharacter != nullptr)
	{
		if (MyCharacter->PoisonConfig.bIsPoisoning)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Character->PoisonConfig->bIsPoisonning"));
			if (TargettedPipe != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("CheckForPoisonned: LowerPipe? %s"), TargettedPipe->bLowerPipe ? TEXT("true") : TEXT("False"));
				if (TargettedPipe->bLowerPipe)
				{
					CurrentAnimationState = AnimationStates::E_AnimRecoilLower;
				}
				else
				{
					CurrentAnimationState = AnimationStates::E_AnimRecoil;
				}
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("CheckForPoisonned: TargettedPipe null"));
				// Bad - but might work
				CurrentAnimationState = AnimationStates::E_AnimRecoil;
			}
			SufferDamage(DeltaTime * NeedleStabDamage);
		}
	}
}

void AQuackBoss::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Method for logic when boss initiates attack
	// Using same class for walls, since it wants identical logic
	// So added one utility function to it, to make a distinction
	ADestructibleInnerPipes* Pipe = Cast<ADestructibleInnerPipes>(OtherActor);
	if (Pipe != nullptr)
	{
		// Prevents melee attacks breaking the fences
		if (!Pipe->IsFenceWall() || CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingTwo
			|| CurrentBossState == BossStates::E_HealingThree || CurrentBossState == BossStates::E_HealingFour)
		{
			Pipe->Fracture();
		}
	}

	AQuackCharacter* _Character = Cast<AQuackCharacter>(OtherActor);
	if (_Character != nullptr)
	{
		// DONE
		// Somehow base cooldown on arm swing anim ???
		// Add a check to !Recoiling so tongue doesnt hurt u when u poison it
		if (CurrentBossState == BossStates::E_Recoiling || CurrentBossState == BossStates::E_Poisoned || CurrentBossState == BossStates::E_HealingOne
			|| CurrentBossState == BossStates::E_HealingTwo || CurrentBossState == BossStates::E_HealingThree || CurrentBossState == BossStates::E_HealingFour)
		{
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("MeleeAttacking: %s"), bMeleeAttacking ? TEXT("TRUE") : TEXT("FALSE"));
			if (!bMeleeAttacking)
			{
				bMeleeAttacking = true;
				_Character->DecreaseHealth(ArmDamage);
				UWorld* const World = GetWorld();
				if (World != nullptr)
				{
					if (!World->GetTimerManager().IsTimerActive(CharHitMeleeTimerHandle))
					{
						World->GetTimerManager().SetTimer(CharHitMeleeTimerHandle, this, &AQuackBoss::EnableMelee, 2.2f, false);
					}
				}
			}
		}

		
		// Maybe add knockback here - once the whole is in the ground.
		// Since currently, you can go through the boss
		// on the ground, and trigger this
	}
}

FRotator AQuackBoss::RotateHeadToPlayer()
{
	if (MyCharacter != nullptr /*&& !bHealingOnPipe && !bPoisonned*/)
	{
		FRotator MyRotation = GetActorRotation();
		const FVector MyLocation = GetActorLocation();
		//// Get other transform
		const FVector OtherLocation = MyCharacter->GetActorLocation();
		//// Find vector that connects the transforms
		FVector Direction = OtherLocation - MyLocation;
		// Project it to 2D
		Direction = FVector(Direction.X, Direction.Y, 0.0f);
		FRotator EndRotation = Direction.Rotation();
		//HeadRot = FMath::RInterpTo(MyRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 0.5f);
		HeadRot = EndRotation;
		// Lerp Negaively
		if (HeadRot.Yaw <= 360.0f && HeadRot.Yaw >= 315.0f || HeadRot.Yaw < 0.0f && HeadRot.Yaw >= -45.0f)
		{
			return HeadRot;
		}
		// Lerp positively
		else if (HeadRot.Yaw <= 45.0f && HeadRot.Yaw >= 0.0f)
		{
			return HeadRot;
		}
		else if (HeadRot.Yaw <= 180.0f && HeadRot.Yaw > 0.0f)
		{
			// Clamping positively
			HeadRot.Yaw = 45.0f;
			return HeadRot;
		}
		else if (HeadRot.Yaw > 180.0f || HeadRot.Yaw < 0.0f && HeadRot.Yaw >= -45.0f)
		{
			// Clamping negatively
			HeadRot.Yaw = -45.0f;
			return HeadRot;
		}
		else
		{
			// Failed Turn
			return FRotator(0.0f);
		}
		//HeadRot.Yaw = FMath::Clamp(HeadRot.Yaw, -45.0f, 45.0f);
		//return HeadRot;
	}
	else
	{
		// Failed Turn
		return FRotator(0.0f);
	}
}

void AQuackBoss::RotateTowardsPlayer()
{
	if (bIsStabbingAnimation) return;
	//UE_LOG(LogTemp, Warning, TEXT("bMeleeStabbing: %s, bMeleeAttacking: %s"), bMeleeStabbing ? TEXT("MeleeStabTrue") : TEXT("MeleeStabFalse"), bMeleeAttacking ? TEXT("MelATKTrue") : TEXT("MelATKFalse"));
	if (MyCharacter != nullptr /*&& !bHealingOnPipe && !bPoisonned*/)
	{
		/*
		// Make sure no pipes are currently being targetted
		//bPipeSelected = false;
		// IMPORTANT TO NOTE, STATIC/SKELETAL MESH MUST BE ORIENTATED CORRECTLY IN BP EXTENSION
		//// Get your transform
		const FVector MyLocation = GetActorLocation();
		//// Get other transform
		const FVector OtherLocation = MyCharacter->GetActorLocation();
		//// Find vector that connects the transforms
		FVector Direction =  MyLocation - OtherLocation;
		// Project it to 2D
		Direction = FVector(Direction.X, Direction.Y, 0.0f);
		// Apply rotation
		SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
		*/

		FRotator MyRotation = GetActorRotation();
		const FVector MyLocation = GetActorLocation();
		//// Get other transform
		const FVector OtherLocation = MyCharacter->GetActorLocation();
		//// Find vector that connects the transforms
		FVector Direction = MyLocation - OtherLocation;
		// Project it to 2D
		Direction = FVector(Direction.X, Direction.Y, 0.0f);
		FRotator EndRotation = Direction.Rotation();
		MyRotation = FMath::RInterpTo(MyRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 1.5f);
		SetActorRotation(MyRotation);
		// Allow Horizontal strafe when rotation towards player
		bStopChase = false;
	}
}

void AQuackBoss::RotateTowardsPipe()
{
	// Go to Centre if not already - ready for pipe
	bStopChase = true;

	FRotator MyRotation = GetActorRotation();
	const FVector MyLocation = GetActorLocation();
	//// Get other transform
	const FVector OtherLocation = CurrentTargettedPipeTransform.TargettedTransform.GetLocation();
	//// Find vector that connects the transforms
	FVector Direction = MyLocation - OtherLocation;
	// Project it to 2D
	Direction = FVector(Direction.X, Direction.Y, 0.0f);
	FRotator EndRotation = Direction.Rotation();
	MyRotation = FMath::RInterpTo(MyRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 1.75f);
	SetActorRotation(MyRotation);
	//UE_LOG(LogTemp, Warning, TEXT("MyRotation: %s , Pipe rotation: %s"), *MyRotation.ToString(), *EndRotation.ToString());
	if (FMath::IsNearlyEqual(MyRotation.Yaw, EndRotation.Yaw, 1.0f))
	{
		if (CurrentTargettedPipeTransform.bIsLowerPipe)
		{
			bFacingTargettedPipeLower = true;
		}
		else
		{
			bFacingTargettedPipe = true;
		}
	}
	else
	{
		bFacingTargettedPipe = false;
		bFacingTargettedPipeLower = false;
	}
}

void AQuackBoss::BeginPipeDrain()
{
	if (TargettedPipe != nullptr)
	{
		TargettedPipe->SetDescend(true);
		//AQuackCharacter* TempCharacter = Cast<AQuackCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
		//if (TempCharacter != nullptr)
		//{
		//	TempCharacter->ForceNeedleGun();
		//	UE_LOG(LogTemp, Warning, TEXT("BeginPipeDrain: + ForceNeedleSwap"));
		//}
	}
}

void AQuackBoss::EndPipeDrain()
{
	if (TargettedPipe != nullptr)
	{
		if(!TargettedPipe->bPoisonedPipe)
		TargettedPipe->SetDescend(false);
	}
}

void AQuackBoss::SetPreviousPipe()
{
	PreviousPipe = TargettedPipe;
}

void AQuackBoss::DestroyPreviousTargettedPipe()
{
	if (PreviousPipe != nullptr)
	{
		PreviousPipe->SimulateDestroy();
		//PreviousPipe->Destroy();
	}
}

void AQuackBoss::LocateNearbyPipe()
{
	//if (!bPipeSelected)
	//{
	//bPipeSelected = true;
	if (LowerPipes.Num() != 0)
	{
		int Random = FMath::RandRange(0, LowerPipes.Num() - 1);
		TargettedPipe = LowerPipes[Random];
		SetPreviousPipe();
		if (TargettedPipe != nullptr)
		{
			LowerPipes.Remove(TargettedPipe);
			TargettedPipe->bTargettedByBoss = true;
			CurrentTargettedPipeTransform = FPipeTransform(TargettedPipe->GetActorTransform(), TargettedPipe->bLowerPipe, TargettedPipe->bLeftPipe);
		}
		// Search for a pipe in scene
		// and return the transform of it
		//CurrentTargettedPipeTransform = PipeTransform;
	}
	else if (UpperPipes.Num() != 0)
	{
		int Random = FMath::RandRange(0, UpperPipes.Num() - 1);
		TargettedPipe = UpperPipes[Random];
		SetPreviousPipe();
		if (TargettedPipe != nullptr)
		{
			UpperPipes.Remove(TargettedPipe);
			TargettedPipe->bTargettedByBoss = true;
			CurrentTargettedPipeTransform = FPipeTransform(TargettedPipe->GetActorTransform(), TargettedPipe->bLowerPipe, TargettedPipe->bLeftPipe);
		}
	}
	//}
}

void AQuackBoss::ChangeState(BossStates DesiredState)
{
	DisableBeam();
	// MIGHT WANNA CHANGE THIS TO ONLY STORE PREV STATE, IF STATE CHANGE CAME FROM A FIGHTING STATE, 
	// SINCE THATS WHAT IT IS RETURNING TOO. 
	// NEED TO TEST - LET BOSS FULLY HEAL IN EACH PHASE
	if (CurrentBossState == BossStates::E_Fighting || CurrentBossState == BossStates::E_FightingTwo || CurrentBossState == BossStates::E_FightingThree || CurrentBossState == BossStates::E_FightingFour)
	{
		PreviousBossState = CurrentBossState;
	}
	RunningTime = 0.0f;
	FireCooldown = InitialFireCooldown;
	StopBileShot();
	StopTailShot();
	StopBileSpray();
	if(bUsingPrimaryAttack)
		ResetMultipleAttacksPattern();
	if (bMeleeStabbing)
		ClearStabRoutine();
	//if(bIsSpawning)
	//	ClearWaveSpawningTimer();
	if (CurrentBossState != DesiredState)
	{
		if (CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingTwo || CurrentBossState == BossStates::E_HealingThree || CurrentBossState == BossStates::E_HealingFour)
		{
			CurrentBossState = DesiredState;
			AQuackCharacter* TempCharacter = Cast<AQuackCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
			if (TempCharacter != nullptr)
			{
				TempCharacter->UnforceNeedleGun();
				EMPParticleSystem->SetActive(false);
				UE_LOG(LogTemp, Warning, TEXT("ChangeState: UnForce"));
			}
		}
		else
		{
			CurrentBossState = DesiredState;
		}
	}
}

void AQuackBoss::SlamGround()
{
	//SLAMEBERRY		
}
void AQuackBoss::StartBileShot(float OverridenFireRate)
{
	if (BossAttacksComponent != nullptr)
	{
		if (MouthArrow == nullptr) return;
		UAnimInstance* AnimInst = MySkeletalMesh->GetAnimInstance();
		if (AnimInst != nullptr && AnimationComponent != nullptr)
		{
			BossAttacksComponent->StartBileSpitting(MouthArrow, AnimInst, AnimationComponent);
		}
	}
}

void AQuackBoss::StopBileShot()
{
	if (BossAttacksComponent != nullptr)
	{
		BossAttacksComponent->StopBileSpitting();
	}
}

void AQuackBoss::StartTailShot()
{
	if (BossAttacksComponent != nullptr)
	{
		if (TailArrow == nullptr) return;
		UAnimInstance* AnimInst = MySkeletalMesh->GetAnimInstance();
		if (AnimInst != nullptr && AnimationComponent != nullptr)
		{
			BossAttacksComponent->StartTailShooting(TailArrow, AnimInst, AnimationComponent);
			CurrentAnimationState = AnimationStates::E_AnimTailShot;
		}
	}
}

void AQuackBoss::StopTailShot()
{
	if (BossAttacksComponent != nullptr)
	{
		BossAttacksComponent->StopTailShoot();
	}
}

void AQuackBoss::StartBileSpray(float OverridenFireRate)
{
	if (BossAttacksComponent != nullptr)
	{
		if (MouthArrow == nullptr) return;
		UAnimInstance* AnimInst = MySkeletalMesh->GetAnimInstance();
		if (AnimInst != nullptr && AnimationComponent != nullptr)
		{
			BossAttacksComponent->StartBileSpraying(MouthArrow, AnimInst, AnimationComponent);
		}
	}
}

void AQuackBoss::StopBileSpray()
{
	if (BossAttacksComponent != nullptr)
	{
		BossAttacksComponent->StopBileSpraying();
	}
}
void AQuackBoss::RotateTowardsWall()
{

}
void AQuackBoss::LocateNearbyWall()
{

}
void AQuackBoss::SmashWall()
{

}