// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/Pipe.h"
#include "Headers/Character/QuackCharacter.h"


// Sets default values
APipe::APipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = MyRoot;

	MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractTrigger"));
	MyTrigger->SetupAttachment(MyRoot);

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipeMesh"));
	MyStaticMesh->SetupAttachment(MyTrigger);

	PipeOuterBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OuterPipe"));
	PipeOuterBase->SetupAttachment(MyTrigger);

	DrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrainMesh"));
	DrainMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DrainMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DrainMesh->SetupAttachment(MyTrigger);

	LeakParticleSys = CreateAbstractDefaultSubobject<UParticleSystemComponent>(TEXT("Leak"));
	LeakParticleSys->SetupAttachment(MyTrigger);
	LeakParticleSys->SetRelativeLocation(PipeOuterBase->RelativeLocation);


}

// Called when the game starts or when spawned
void APipe::BeginPlay()
{
	Super::BeginPlay();
	if (MyTrigger != nullptr)
	{
		MyTrigger->bGenerateOverlapEvents = true;
		MyTrigger->OnComponentBeginOverlap.AddDynamic(this, &APipe::OnTriggerEnter);
		MyTrigger->OnComponentEndOverlap.AddDynamic(this, &APipe::OnTriggerExit);
	}
	LeakParticleSys->Deactivate();
	DrainMesh->SetVisibility(false);
	SpawnLocation = MyStaticMesh->GetComponentLocation();
}

// Called every frame
void APipe::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (bDescend)
	{
		BeginDescend(DeltaTime);
	}
	else
	{
		BeginAscend(DeltaTime);
	}

	if (bDrained && bDestroyed)
	{
		if (MyStaticMesh != nullptr)
		{
			MyStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MyStaticMesh->SetHiddenInGame(true);
			DrainMesh->SetVisibility(false);
			LeakParticleSys->Deactivate();
			bFinishedDraining = true;
			if (OnPipeFinishedDraining.IsBound())
			{
				OnPipeFinishedDraining.Broadcast();
				//UE_LOG(LogTemp, Warning, TEXT("Broadcasting"));
				OnPipeFinishedDraining.RemoveAll(this);
			}
			bDrained = false;
			bDestroyed = false;
		}
	}
}

void APipe::SetDescend(bool _State)
{
	bDescend = _State;
}

void APipe::BeginDescend(float DeltaTime)
{
	if (MyStaticMesh == nullptr) return;
	//FVector CurrentLocation = MyStaticMesh->GetComponentLocation();
	//CurrentLocation = FMath::VInterpTo(CurrentLocation, FVector(SpawnLocation.X, SpawnLocation.Y, MinPipeHeight), DeltaTime, DrainSpeed);
	//MyStaticMesh->SetRelativeLocation(CurrentLocation);

	float CurZ = MyStaticMesh->GetRelativeTransform().GetLocation().Z;

	CurZ = FMath::FInterpConstantTo(CurZ, MinPipeHeight, DeltaTime, DrainSpeed);
	MyStaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, CurZ));

	if (CurZ <= MinPipeHeight + 10.0f)
	{
		bDrained = true;
	}
	//SetActorLocation(CurrentLocation);
	
}

void APipe::BeginAscend(float DeltaTime)
{
	if (MyStaticMesh == nullptr) return;
	//FVector CurrentLocation = MyStaticMesh->GetComponentLocation();
	//CurrentLocation = FMath::VInterpTo(CurrentLocation, FVector(SpawnLocation.X, SpawnLocation.Y, MaxPipeHeight), DeltaTime, DrainSpeed);
	//MyStaticMesh->SetRelativeLocation(CurrentLocation);

	float CurZ = MyStaticMesh->GetRelativeTransform().GetLocation().Z;
	CurZ = FMath::FInterpConstantTo(CurZ, MaxPipeHeight, DeltaTime, DrainSpeed / 2.0f);
	MyStaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, CurZ));
	//SetActorLocation(CurrentLocation);
}

void APipe::SimulateDestroy()
{
	bDescend = true;
	bDestroyed = true;
	DrainMesh->SetVisibility(true);
	LeakParticleSys->ActivateSystem();
	

	//MyStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//MyStaticMesh->SetHiddenInGame(true);
}

void APipe::ChangeMeshColour()
{
	//PipeOuterBase->SetMaterial(0, PoisonnedPipeMaterial);
}

void APipe::ToggleHighlight(bool Glow)
{
	//MyStaticMesh->SetCustomDepthStencilValue((uint8)Colour);
	PipeOuterBase->SetRenderCustomDepth(Glow);

}

void APipe::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Poison Pipe enter"));
		// Poisonable
		if (bNotABossPipe) return;
		if (!bPoisonedPipe && bTargettedByBoss)
		{
			MyCharacter->PoisonConfig.bCanPoisonPipe = true;
			ToggleHighlight(true);
		}
		else
		{
			ToggleHighlight(false);
		}
		//Destroy();
	}
}

void APipe::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Poison Pipe exit"));
		// Poisonable
		MyCharacter->PoisonConfig.bCanPoisonPipe = false;
		ToggleHighlight(false);

		//Destroy();
	}
}