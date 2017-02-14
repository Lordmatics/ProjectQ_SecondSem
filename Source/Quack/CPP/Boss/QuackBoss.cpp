
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
	BodyPlate->SetupAttachment(MySkeletalMesh);

	BodyUL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyUL"));
	BodyUL->SetWorldScale3D(FVector(1.0f));
	BodyUL->SetupAttachment(BodyPlate);

	BodyUR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyUR"));
	BodyUR->SetWorldScale3D(FVector(1.0f));
	BodyUR->SetupAttachment(BodyPlate);

	BodyLL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyLL"));
	BodyLL->SetWorldScale3D(FVector(1.0f));
	BodyLL->SetupAttachment(BodyPlate);

	BodyLR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyLR"));
	BodyLR->SetWorldScale3D(FVector(1.0f));
	BodyLR->SetupAttachment(BodyPlate);

	LaserCannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserCannon"));
	LaserCannon->SetWorldScale3D(FVector(100.0f));
	LaserCannon->SetRelativeLocation(FVector(-0.002477f, 54.748207f, 832.158630f));
	LaserCannon->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	LaserCannon->SetupAttachment(MySkeletalMesh);


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

	InitialisePlayerCharacterReference();
	InitialiseRockSpawnerReference();

	MinionFactory = CreateDefaultSubobject<UMinionFactoryComponent>(TEXT("MinionFactory"));
	ChandelierDropComponent = CreateDefaultSubobject<UVerticalMovementComponent>(TEXT("ChandelierDropComponent"));

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AAudioManager> ActorItr(World); ActorItr; ++ActorItr)
		{
			AudioManager = *ActorItr;
		}
	}

}

void AQuackBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Obtain Static Meshes References
	// DEPRECATED - Except sphere shield
	//InitialiseStaticMeshes();
	///InitialiseSkeletalMeshes();
	///InitialiseArrowComponent();
	// Delegate collision onto the boss mesh
	MySkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &AQuackBoss::OnTriggerEnter);
	///if (SkeletalMeshes.Num() == 0) return;
	///SkeletalMeshes[0]->OnComponentBeginOverlap.AddDynamic(this, &AQuackBoss::OnTriggerEnter);
	//if (SkeletalMesh != nullptr)
	//{
	//	SkeletalMesh->bGenerateOverlapEvents = true;
	//	SkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &AQuackBoss::OnTriggerEnter);
	//}
	//if (StaticMeshes.Num() == 0) return;
	//StaticMeshes[0]->OnComponentBeginOverlap.AddDynamic(this, &AQuackBoss::OnTriggerEnter);
}

// Called when the game starts or when spawned
void AQuackBoss::BeginPlay()
{
	Super::BeginPlay();
	MaxBossHealth = BossHealth;
	InitialFireCooldown = FireCooldown;
	CurrentBossState = BossStates::E_Idle;
	if (MyCharacter != nullptr)
	{
		MyCharacter->BossHP = MaxBossHealth / 100.0f;
	}
	LocateNearbyPipe();
	ToggleShield(false);

	TailArrow->AttachToComponent(MySkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), TEXT("Tail6"));
	//TailArrow->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));
//	TailArrow->SetRelativeLocation(FVector(0.0f, 0.0f , 50.0f));

	MouthArrow->AttachToComponent(MySkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), TEXT("Mouth"));
	//MouthArrow->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));
//	MouthArrow->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));


}

void AQuackBoss::ResumeFighting()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Resume fighting"));
		FTimerHandle TempTimer;
		World->GetTimerManager().SetTimer(TempTimer, this, &AQuackBoss::ChangeBackToPrevious, RecoilDuration, false);
	}
}

void AQuackBoss::ChangeBackToPrevious()
{
	UE_LOG(LogTemp, Warning, TEXT("Resume fighting 2"));

	if (PreviousBossState != BossStates::E_Fighting || PreviousBossState != BossStates::E_FightingTwo || PreviousBossState != BossStates::E_FightingThree || PreviousBossState != BossStates::E_FightingFour)
	{
		ChangeState(BossStates::E_FightingTwo);
	}
	else
	{
		ChangeState(PreviousBossState);

	}
}

// Called every frame
void AQuackBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BossHealth >= MaxBossHealth)
	{
		// THIS IS SOMEWHAT OF AN EXPERIEMENT ... PLIS WORK -- RECOILD WHEN FULLY HEALED
		//CurrentAnimationState = AnimationStates::E_AnimRecoil;		
		if (CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingTwo || CurrentBossState == BossStates::E_HealingThree || CurrentBossState == BossStates::E_HealingFour || CurrentBossState == BossStates::E_Poisoned)
		{
			ChangeState(BossStates::E_Recoiling);
			ResumeFighting();
		}
		else
		{
			ChangeState(BossStates::E_Idle);
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
	//if (StaticMeshes.Num() >= 1)
	//{
	//	if (StaticMeshes[0] == nullptr) return;
	//	StaticMeshes[0]->SetVisibility(Visiblity);
	//	bShieldUp = Visiblity;
	//}
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
			//if (BossHealth >= MaxBossHealth)
			//{
			//	CurrentAnimationState = AnimationStates::E_AnimRecoil;
			//}
			//else
			//{
			CurrentAnimationState = AnimationStates::E_AnimIdle;
			//}
			/*RotateTowardsPlayer();
			ToggleShield(false);
			break;*/
			// Below 80% phase one healing
			if (Pipes.Num() + 1 == 4 && TargettedPipe != nullptr)
			{
				ChangeState(BossStates::E_Fighting);
			}
			// Below 60% Phase two healing
			else if (Pipes.Num() + 1 == 3 && TargettedPipe != nullptr)
			{
				ChangeState(BossStates::E_FightingTwo);
			}
			// Below 40% Phase three healing
			else if (Pipes.Num() + 1 == 2 && TargettedPipe != nullptr)
			{
				ChangeState(BossStates::E_FightingThree);
			}
			// Below 20% Phase four healing
			else if (Pipes.Num() + 1 == 1 && TargettedPipe != nullptr)
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
			CurrentAnimationState = AnimationStates::E_AnimLatch;
			RotateTowardsPipe();
			BeginPipeDrain();

			Regenerate(DeltaTime);
			ToggleShield(true);
			break;
		}
		case BossStates::E_HealingTwo:
		{
			CurrentAnimationState = AnimationStates::E_AnimLatch;
			RotateTowardsPipe();
			BeginPipeDrain();

			Regenerate(DeltaTime);
			ToggleShield(true);
			break;
		}
		case BossStates::E_HealingThree:
		{
			CurrentAnimationState = AnimationStates::E_AnimLatch;
			RotateTowardsPipe();
			BeginPipeDrain();

			Regenerate(DeltaTime);
			ToggleShield(true);
			break;
		}
		case BossStates::E_HealingFour:
		{
			CurrentAnimationState = AnimationStates::E_AnimLatch;
			//CurrentAnimationState = AnimationStates::E_AnimGulp;
			RotateTowardsPipe();
			BeginPipeDrain();
			Regenerate(DeltaTime);
			ToggleShield(true);
			break;
		}
		case BossStates::E_Poisoned:
		{
			RotateTowardsPipe();
			CheckForPoisoned(DeltaTime);
			ToggleShield(false);
			EndPipeDrain();
			break;
		}
		case BossStates::E_Recoiling:
		{
			CurrentAnimationState = AnimationStates::E_AnimRecoil;
			RotateTowardsPlayer();
			ToggleShield(false);
			EndPipeDrain();
			break;
		}
		case BossStates::E_Fighting:
		{
			//ShootFromTail(DeltaTime);
			ToggleShield(false);
			RotateTowardsPlayer();
			//SlamGround();
			//ShootFromTail(DeltaTime);
			//BeginMultipleAttacksPattern();
			if (!CheckForMeleeAttack())
			{
				StartTailShot();
			}

											//StartTailShot();

			//ShootFromTail(DeltaTime);

			break;
		}
		case BossStates::E_FightingTwo:
		{
			ToggleShield(false);
			RotateTowardsPlayer();
			//SlamGround();
			//ShootBile(DeltaTime);
			//ShootFromTail(DeltaTime);
			//ShootBile(DeltaTime);
			if (!CheckForMeleeAttack())
			{
				BeginMultipleAttacksPattern();
			}
			//StartBileShot();

			break;
		}
		case BossStates::E_FightingThree:
		{

			RotateTowardsPlayer();
			//SlamGround();
			//ShootBile(DeltaTime);
			//ShootFromTail(DeltaTime);

			// demonstration only - change to melee when u get too close - subject to change
			//CurrentAnimationState = AnimationStates::E_AnimMelee;
			//CheckForMeleeAttack();
			//bool Check = CheckForMeleeAttack();
			//UE_LOG(LogTemp, Warning, TEXT("Fighting Three : Check for Melee Attack : %s"), Check ? TEXT("true") : TEXT("false"));
			if (!CheckForMeleeAttack())
			{
				// Bile during spawn phase, but half as often - balance
				StartBileShot(BileFireRate / 2);
			}
			BeginWaveSpawningCycle();
			if (MinionFactory != nullptr)
			{
				switch (MinionFactory->AreMinionsAlive())
				{
					case true:
					{
						if (ChandelierDropComponent != nullptr)
						{
							ChandelierDropComponent->Lower();
						}
						ToggleShield(true);
						break;
					}
					case false:
					{
						if (ChandelierDropComponent != nullptr)
						{
							ChandelierDropComponent->Raise();
						}
						ToggleShield(false);
						break;
					}
				}
			}
			break;
		}
		case BossStates::E_FightingFour:
		{

			RotateTowardsPlayer();
			if (!CheckForMeleeAttack())
			{
				BeginMultipleAttacksPattern();
			}
			BeginWaveSpawningCycle();
			if (MinionFactory != nullptr)
			{
				switch (MinionFactory->AreMinionsAlive())
				{
				case true:
				{
					if (ChandelierDropComponent != nullptr)
					{
						ChandelierDropComponent->Lower();
					}
					ToggleShield(true);
					break;
				}
				case false:
				{
					if (ChandelierDropComponent != nullptr)
					{
						ChandelierDropComponent->Raise();
					}
					ToggleShield(false);
					break;
				}
				}
			}
			break;
		}
	}
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
		UE_LOG(LogTemp, Warning, TEXT("BeginWaveSpawning : Phase Fighting Three"));
	//	UE_LOG(LogTemp, Warning, TEXT("BeginSpawningCycle"));
		//ChangeAttack();
		SpawnMinions();
		World->GetTimerManager().SetTimer(WaveSpawnerTimer, this, &AQuackBoss::SpawnMinions, WaveSpawnDelay, true);
		bIsSpawning = true;
	}
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
		if (MinionFactory->AreMinionsAlive())
		{
			return;
		}
		WaveSpawnCounter++;
		if (WaveSpawnCounter % 2 == 0)
		{
			MinionFactory->SpawnMinionWaveA();
		}
		else
		{
			MinionFactory->SpawnMinionWaveB();
		}
	}
}

void AQuackBoss::ClearWaveSpawningTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Spawn Timer RESET : Boss"));
	bIsSpawning = false;
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
	if (bIsBileSpitting)
	{
		StopBileShot();
		StartTailShot();
	}
	else if(bIsTailShooting)
	{
		StopTailShot();
		// set bilespitting to true once then repeat the attack function
		StartBileShot();
	}
	else
	{
		// This case is coz when he resets his phase, both attacks are false so, in that event, choose one randomly
		int Rand = FMath::RandRange(0, 3);
		if (Rand == 0)
		{
			StopTailShot();
			StartBileShot();
		}
		else
		{
			StartBileShot();
			StopTailShot();
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
	if (Pipes.Num() == 0 && TargettedPipe == nullptr)
	{
		//ChangeState(BossStates::E_Idle);
		return;
	}
	else
	{
		// Below 80% phase one healing
		if (BossHealth <= ((4 * MaxBossHealth) / 5) && Pipes.Num() + 1 == 4 && TargettedPipe != nullptr)
		{
			ChangeState(BossStates::E_HealingOne);
		}
		// Below 60% Phase two healing
		else if (BossHealth <= ((3 * MaxBossHealth) / 5) && Pipes.Num() + 1 == 3 && TargettedPipe != nullptr)
		{
			ChangeState(BossStates::E_HealingTwo);
		}
		// Below 40% Phase three healing
		else if (BossHealth <= (2 * MaxBossHealth / 5) && Pipes.Num() + 1 == 2 && TargettedPipe != nullptr)
		{
			ChangeState(BossStates::E_HealingThree);
		}
		// Below 20% Phase four healing
		else if (BossHealth <= (MaxBossHealth / 5) && Pipes.Num() + 1 == 1 && TargettedPipe != nullptr)
		{
			ChangeState(BossStates::E_HealingFour);
		}
	}
}
void AQuackBoss::InitialiseStaticMeshes()
{
	// Obtain references to all static meshes on this blueprint extension
	// First index will be the sphere mesh
	TArray<UActorComponent*> Components = GetComponentsByClass(UStaticMeshComponent::StaticClass());

	if (Components.Num() == 0) return;
	for (size_t i = 0; i < Components.Num(); i++)
	{
		UStaticMeshComponent* BossMesh = Cast<UStaticMeshComponent>(Components[i]);
		if (BossMesh != nullptr)
		{
			//BossMesh->bGenerateOverlapEvents = true;
			StaticMeshes.Add(BossMesh);
		}
	}
}

void AQuackBoss::InitialiseSkeletalMeshes()
{
	//Obtain references to all skeletal meshes on this blueprint extension
	//First index will be the boss mesh
	TArray<UActorComponent*> Components = GetComponentsByClass(USkeletalMeshComponent::StaticClass());

	if (Components.Num() == 0) return;
	for (size_t i = 0; i < Components.Num(); i++)
	{
		USkeletalMeshComponent* BossMesh = Cast<USkeletalMeshComponent>(Components[i]);
		if (BossMesh != nullptr)
		{
			BossMesh->bGenerateOverlapEvents = true;
			SkeletalMeshes.Add(BossMesh);
		}
	}
}

void AQuackBoss::InitialiseArrowComponent()
{
	TArray<UActorComponent*> Components = GetComponentsByClass(UArrowComponent::StaticClass());

	if (Components.Num() == 0) return;
	for (size_t i = 0; i < Components.Num(); i++)
	{
		UArrowComponent* SpawnPoint = Cast<UArrowComponent>(Components[i]);
		if (SpawnPoint != nullptr)
		{
			ProjectileSpawns.Add(SpawnPoint);
		}
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

void AQuackBoss::InitialiseRockSpawnerReference()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<ARockSpawner> ActorItr(World); ActorItr; ++ActorItr)
		{
			RockSpawner = *ActorItr;
		}
	}
}

void AQuackBoss::SufferDamage(float Amount)
{
	if (CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingTwo || CurrentBossState == BossStates::E_HealingThree || CurrentBossState == BossStates::E_HealingFour) return;
	if (bShieldUp) return;
	// Utility Function to Deduct Damage from Health then clamp for safety
	BossHealth -= Amount;
	if (MyCharacter != nullptr)
		MyCharacter->BossHP = BossHealth / MaxBossHealth;
	BossHealth = FMath::Clamp(BossHealth, 0.0f, MaxBossHealth);
	CheckForDead();
}

void AQuackBoss::Regenerate(float DeltaTime)
{
	// Used when boss enters regen phase ---- Poison Mechanic
	//if (!bIsRegenerating) return;
	//if (bPoisonned) return;
	BossHealth += BossRegenRate * DeltaTime;
	if (MyCharacter != nullptr)
		MyCharacter->BossHP = BossHealth / MaxBossHealth;
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
		Destroy();
	}
}

void AQuackBoss::CheckForPoisoned(float DeltaTime)
{
	if (MyCharacter != nullptr)
	{
		if (MyCharacter->PoisonConfig.bIsPoisoning)
		{
			CurrentAnimationState = AnimationStates::E_AnimRecoil;
			SufferDamage(DeltaTime * 5.0f);
		}
	}
}

void AQuackBoss::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Method for logic when boss initiates attack
	ADestructibleInnerPipes* Pipe = Cast<ADestructibleInnerPipes>(OtherActor);
	if (Pipe != nullptr)
	{
		Pipe->Fracture();
	}

	AQuackCharacter* _Character = Cast<AQuackCharacter>(OtherActor);
	if (_Character != nullptr)
	{
		// Somehow base cooldown on arm swing anim ???
		if (!bMeleeAttacking)
		{
			bMeleeAttacking = true;
			_Character->DecreaseHealth(ArmDamage);
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				if (!World->GetTimerManager().IsTimerActive(CharHitMeleeTimerHandle))
				{
					World->GetTimerManager().SetTimer(CharHitMeleeTimerHandle, this, &AQuackBoss::EnableMelee, 1.75f, false);
				}
			}
		}
		// Maybe add knockback here - once the whole is in the ground.
		// Since currently, you can go through the boss
		// on the ground, and trigger this
	}
}

void AQuackBoss::EnableMelee()
{
	UE_LOG(LogTemp, Warning, TEXT("Melee Reset"));
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().ClearTimer(CharHitMeleeTimerHandle);
	bMeleeAttacking = false;
	//bMeleeStarted = false;
	//bMeleeTimerTicking = false;
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
		MyRotation = FMath::RInterpTo(MyRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 2);
		SetActorRotation(MyRotation);
	}
}

void AQuackBoss::RotateTowardsPipe()
{
	/*
	//if (!bHealingOnPipe) return;
	// IMPORTANT TO NOTE, STATIC/SKELETAL MESH MUST BE ORIENTATED CORRECTLY IN BP EXTENSION
	//// Get your transform
	const FVector MyLocation = GetActorLocation();
	//// Get other transform
	const FVector OtherLocation = CurrentTargettedPipeTransform.GetLocation();
	//// Find vector that connects the transforms
	FVector Direction = MyLocation - OtherLocation;
	// Project it to 2D
	Direction = FVector(Direction.X, Direction.Y, 0.0f);
	// Apply rotation
	SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	*/

	FRotator MyRotation = GetActorRotation();
	const FVector MyLocation = GetActorLocation();
	//// Get other transform
	const FVector OtherLocation = CurrentTargettedPipeTransform.GetLocation();
	//// Find vector that connects the transforms
	FVector Direction = MyLocation - OtherLocation;
	// Project it to 2D
	Direction = FVector(Direction.X, Direction.Y, 0.0f);
	FRotator EndRotation = Direction.Rotation();
	MyRotation = FMath::RInterpTo(MyRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 2);
	SetActorRotation(MyRotation);
}

void AQuackBoss::BeginPipeDrain()
{
	
	if (TargettedPipe != nullptr)
	{
		TargettedPipe->SetDescend(true);
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
	if (Pipes.Num() != 0)
	{
		int Random = FMath::RandRange(0, Pipes.Num() - 1);
		TargettedPipe = Pipes[Random];
		SetPreviousPipe();
		if (TargettedPipe != nullptr)
		{
			Pipes.Remove(TargettedPipe);
			TargettedPipe->bTargettedByBoss = true;
			CurrentTargettedPipeTransform = TargettedPipe->GetActorTransform();
		}
		// Search for a pipe in scene
		// and return the transform of it
		//CurrentTargettedPipeTransform = PipeTransform;
	}
	//}
}

void AQuackBoss::ChangeState(BossStates DesiredState)
{
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
	if(bUsingPrimaryAttack)
		ResetMultipleAttacksPattern();
	if(bIsSpawning)
		ClearWaveSpawningTimer();
	if (CurrentBossState != DesiredState)
	{
		if (CurrentBossState == BossStates::E_HealingOne || CurrentBossState == BossStates::E_HealingTwo || CurrentBossState == BossStates::E_HealingThree || CurrentBossState == BossStates::E_HealingFour)
		{
			ActivateSomeRocks();
			CurrentBossState = DesiredState;
		}
		else
		{
			CurrentBossState = DesiredState;
		}
	}
}

// DEPRECATED, SINCE WE REMOVED GRAV GUN
void AQuackBoss::ActivateSomeRocks()
{
	//if (RockSpawner != nullptr)
	//{
	//	if (RockSpawner->ActiveRocks.Num() >= 6)
	//	{
	//		int Random = FMath::RandRange(5, RockSpawner->ActiveRocks.Num() - 1);
	//		int MaxRock = Random - 1;
	//		int Index = 0;
	//		for (size_t i = 0; i < Random; i++)
	//		{
	//			Index = FMath::RandRange(0, MaxRock);
	//			if(RockSpawner->ActiveRocks[Index] != nullptr)
	//			RockSpawner->ActiveRocks[Index]->ToggleGravity(true);
	//			int RandomChance = FMath::RandRange(0, 10);
	//			if (RandomChance > 0)
	//			{
	//				if (RockSpawner->ActiveRocks[Index] != nullptr)
	//					RockSpawner->ActiveRocks[Index]->InitiateSelfDestruct();
	//			}
	//			RockSpawner->ActiveRocks.RemoveAt(Index);
	//			RockSpawner->ReplaceFallingRocks(Index);
	//			MaxRock--;
	//		}
	//	}
	//}
}

void AQuackBoss::SlamGround()
{
	//SLAMEBERRY		
}
void AQuackBoss::StartBileShot(float OverridenFireRate)
{
	if (bIsBileSpitting) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		//ShootBile();
		// NOTE: not sure how to make this fire rate match the animation
		// Kris might know
		// Ideally at the end of each loop of the bile spit animation - wanna shoot a projectile, but yeah just guess work here 
		float Rate = 1.0f / BileFireRate;
		if (OverridenFireRate < 1.0f)
		{
		//	UE_LOG(LogTemp, Warning, TEXT("Msg to self: Overriden fire rate success"));
			Rate = 1.0f / OverridenFireRate;

		}
		ShootBile();
		World->GetTimerManager().SetTimer(BileTimer, this, &AQuackBoss::ShootBile, Rate, true);
		bIsBileSpitting = true;
		bWasBileMostRecent = true;
	}
}

void AQuackBoss::ShootBile()
{
	if (MouthArrow != nullptr)
	{
		if (Projectile[1] != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				CurrentAnimationState = AnimationStates::E_AnimBileSpit;
				const FVector Location = MouthArrow->GetComponentLocation();
				const FRotator Rotation = MouthArrow->GetComponentRotation();
				AQuackProjectile* Proj = World->SpawnActor<AQuackProjectile>(Projectile[1], Location, Rotation);
				if (Proj != nullptr)
				{
					float NewScale = 0.5f; // 0.5f * BileShotsFired;

					BileShotsFired++;
					if (BileShotsFired > 3)
						BileShotsFired = 1;
					// far close med
					if (BileShotsFired == 1)
						NewScale = 0.5f;
					else if (BileShotsFired == 2)
						NewScale = 0.25f;
					else if (BileShotsFired == 3)
						NewScale = 0.875f;
					Proj->AdjustProjectileMovementScale(NewScale);
				}
			}
		}
	}
}

void AQuackBoss::StopBileShot()
{
	bIsBileSpitting = false;
	bWasBileMostRecent = false;
	//if (!BileTimer.IsValid()) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(BileTimer);
	}
}

//void AQuackBoss::ShootBile(float DeltaTime)
//{
//	//bleh
//	//if (ProjectileSpawns.Num() == 0) return;
//	//if (ProjectileSpawns[1] != nullptr && Projectile[1] != nullptr)
//	//{
//	//	CurrentAnimationState = AnimationStates::E_AnimBileSpit;
//	//	UWorld* const World = GetWorld();
//	//	if (World != nullptr)
//	//	{
//	//		RunningTime += DeltaTime;
//	//		if (RunningTime >= FireCooldown)
//	//		{
//	//			const FVector Location = ProjectileSpawns[1]->GetComponentLocation();
//	//			const FRotator Rotation = ProjectileSpawns[1]->GetComponentRotation();
//	//			World->SpawnActor<AQuackProjectile>(Projectile[1], Location, Rotation);
//	//			FireCooldown += FireRate;
//	//		}
//	//	}
//	//}
//
//	if (MouthArrow != nullptr)
//	{
//		if (Projectile[1] != nullptr)
//		{
//			CurrentAnimationState = AnimationStates::E_AnimBileSpit;
//			UWorld* const World = GetWorld();
//			if (World != nullptr)
//			{
//				FTimerHandle Timer;
//				World->GetTimerManager().SetTimer(Timer, this, &AQuackBoss::BileRepeat, 1.0f  / BileFireRate, true);
//				RunningTime += DeltaTime;
//				if (RunningTime >= FireCooldown)
//				{
//					const FVector Location = MouthArrow->GetComponentLocation();
//					const FRotator Rotation = MouthArrow->GetComponentRotation();
//					World->SpawnActor<AQuackProjectile>(Projectile[1], Location, Rotation);
//					FireCooldown += FireRate;
//				}
//			}
//		}
//	}
//}

void AQuackBoss::StartTailShot()
{
	if (bIsTailShooting) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		//ShootBile();
		TailShoot();
		World->GetTimerManager().SetTimer(TailTimer, this, &AQuackBoss::TailShoot, 1.0f / TailFireRate, true);
		bIsTailShooting = true;
		bWasTailMostRecent = true;
	}
}

void AQuackBoss::StopTailShot()
{
	bIsTailShooting = false;
	bWasTailMostRecent = false;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(TailTimer);
	}
}

void AQuackBoss::TailShoot()
{
	if (TailArrow != nullptr)
	{
		if (Projectile[0] != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				CurrentAnimationState = AnimationStates::E_AnimTailShot;
				const FVector Location = TailArrow->GetComponentLocation();
				const FRotator Rotation = TailArrow->GetComponentRotation();
				World->SpawnActor<AQuackProjectile>(Projectile[0], Location, Rotation);
			}
		}
	}
}

//void AQuackBoss::ShootFromTail(float DeltaTime)
//{
//	//if (ProjectileSpawns.Num() == 0) return;
//	//if (ProjectileSpawns[0] != nullptr && Projectile[0] != nullptr)
//	//{
//	//	CurrentAnimationState = AnimationStates::E_AnimTailShot;
//	//	UWorld* const World = GetWorld();
//	//	if (World != nullptr)
//	//	{
//	//		RunningTime += DeltaTime;
//	//		if (RunningTime >= FireCooldown)
//	//		{
//	//			const FVector Location = ProjectileSpawns[0]->GetComponentLocation();
//	//			const FRotator Rotation = ProjectileSpawns[0]->GetComponentRotation();
//	//			AQuackProjectile* Proj = World->SpawnActor<AQuackProjectile>(Projectile[0], Location, Rotation);
//	//			if (Proj != nullptr)
//	//			{
//	//				//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Firing")));
//	//				FireCooldown += FireRate;
//	//			}
//	//		}
//
//	//	}
//	//}
//
//	if (TailArrow != nullptr)
//	{
//		if (Projectile[0] != nullptr)
//		{
//			CurrentAnimationState = AnimationStates::E_AnimTailShot;
//			UWorld* const World = GetWorld();
//			if (World != nullptr)
//			{
//				RunningTime += DeltaTime;
//				if (RunningTime >= FireCooldown)
//				{
//					const FVector Location = TailArrow->GetComponentLocation();
//					const FRotator Rotation = TailArrow->GetComponentRotation();
//					AQuackProjectile* Proj = World->SpawnActor<AQuackProjectile>(Projectile[0], Location, Rotation);
//					if (Proj != nullptr)
//					{
//						//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Firing")));
//						FireCooldown += FireRate;
//					}
//				}
//			}
//		}
//	}
//}

void AQuackBoss::RotateTowardsWall()
{

}
void AQuackBoss::LocateNearbyWall()
{

}
void AQuackBoss::SmashWall()
{

}