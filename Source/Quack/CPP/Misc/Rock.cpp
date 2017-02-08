// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/Rock.h"
#include "Headers/Boss/QuackBoss.h"
#include "Headers/CustomComponents/DeathByRock.h"
#include "Headers/Managers/TutorialManager.h"
#include "Headers/CustomComponents/GrabAndThrowComponent.h"

// Sets default values
ARock::ARock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = MyRoot;

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	MyStaticMesh->SetupAttachment(MyRoot);
}

// Called when the game starts or when spawned
void ARock::BeginPlay()
{
	Super::BeginPlay();

	if (MyStaticMesh != nullptr)
	{
		MyStaticMesh->bGenerateOverlapEvents = true;
		MyStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ARock::OnTriggerEnter);
	}
	if (bIsGroundRock)
		ToggleGravity(true);
	else
		ToggleGravity(false);
}

// Called every frame
void ARock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARock::SpawnHitEffect(FVector Location)
{
	// try and play the sound if specified
	if (RockSound != NULL && MyStaticMesh != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RockSound, MyStaticMesh->GetComponentLocation(), 0.1f);
	}

	if (RockParticle != NULL)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, RockParticle, Location, GetActorRotation(), true);
	}
}

void ARock::ToggleGravity(bool Toggle)
{
	if (MyStaticMesh != nullptr)
	{
		MyStaticMesh->SetEnableGravity(Toggle);
		MyStaticMesh->SetSimulatePhysics(Toggle);
	}
}

void ARock::InitiateSelfDestruct()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(SelfDestruct, this, &ARock::DestroyMe, RockLifeTime, false);
	}
}

void ARock::SetGrabOwner(UGrabAndThrowComponent* _GrabOwner)
{
	if(_GrabOwner != nullptr)
		GrabOwner = _GrabOwner;
}

void ARock::ReleaseFromGrabber()
{
	if (GrabOwner != nullptr)
	{
		GrabOwner->MeleeRockRelease();
		//GrabOwner->bPhysicsHandleActive = false;
		//GrabOwner->ReleaseRock();

	}
}

void ARock::DestroyMe()
{
	if (!IsPendingKill() && !bIsHeld)
	{
		ReleaseFromGrabber();
		Destroy();
	}
}

void ARock::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuackBoss* TheBoss = Cast<AQuackBoss>(OtherActor);
	if (TheBoss != nullptr && !IsPendingKill())
	{
		SpawnHitEffect(SweepResult.Location);
		TheBoss->SufferDamage(RockDamage);
		TheBoss->ShouldEnterHealingPhase();
		ReleaseFromGrabber();
		Destroy();
	}
	if (OtherActor != nullptr)
	{
		UDeathByRock* DeathByRockComponent = Cast<UDeathByRock>(OtherActor->GetComponentByClass(UDeathByRock::StaticClass()));
		if (DeathByRockComponent != nullptr)
		{
			if (TutorialManager != nullptr && !bIsHeld)
			{
				TutorialManager->TrialRockThrown();
				SpawnHitEffect(SweepResult.Location);
				ReleaseFromGrabber();
				OtherActor->Destroy();
				Destroy();
			}
		}
	}
}

void ARock::SetHeavyMesh()
{
	if (MyStaticMesh != nullptr)
	{
		MyStaticMesh->SetMaterial(0, HeavyMaterial);
	}
}