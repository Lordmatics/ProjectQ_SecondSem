// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/DestructibleInnerPipes.h"
#include "Components/DestructibleComponent.h"
#include "Classes/PhysicsEngine/RadialForceComponent.h"

// Sets default values
ADestructibleInnerPipes::ADestructibleInnerPipes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = MyRoot;

	MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractTrigger"));
	MyTrigger->SetupAttachment(MyRoot);

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipeMesh"));
	MyStaticMesh->SetupAttachment(MyTrigger);

	MyDestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	MyDestructibleMesh->OnComponentFracture.AddDynamic(this, &ADestructibleInnerPipes::OnComponentFracture);
	MyDestructibleMesh->SetupAttachment(MyTrigger);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(MyDestructibleMesh);
}

// Called when the game starts or when spawned
void ADestructibleInnerPipes::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestructibleInnerPipes::Fracture()
{
	// Called when arm overlaps trigger
	if (MyDestructibleMesh != nullptr && MyStaticMesh != nullptr)
	{
		//MyDestructibleMesh->ApplyRadiusDamage(100, MyStaticMesh->GetComponentLocation(), 35.0f, 15.0f, true);

		//MyDestructibleMesh->ApplyRadiusDamage(100, MyDestructibleMesh->GetComponentLocation(), 35.0f, 15.0f, true);
		//MyDestructibleMesh->ApplyDamage(100.0f, MyDestructibleMesh->GetComponentTransform().GetLocation(), -FVector::UpVector, 1.0f);
		if (RadialForce != nullptr)
		{
			//MyDestructibleMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Ignore);
			RadialForce->FireImpulse();
		}
		//UE_LOG(LogTemp, Warning, TEXT("Should of fractured Pipe"));
	}
}
// Called every frame
void ADestructibleInnerPipes::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ADestructibleInnerPipes::OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (MyStaticMesh != nullptr && bKillStaticMesh)
		{
			MyStaticMesh->DestroyComponent();
		}
		FTimerHandle CollisionTimer;
		World->GetTimerManager().SetTimer(CollisionTimer, this, &ADestructibleInnerPipes::DisableCollision, 0.1f, false);

		FTimerHandle DestroyTimer;
		World->GetTimerManager().SetTimer(DestroyTimer, this, &ADestructibleInnerPipes::DestroyThis, 5.0f, false);

	}

	//MyDestructibleMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//MyDestructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADestructibleInnerPipes::DisableCollision()
{
	if (MyDestructibleMesh != nullptr)
	{
		//MyDestructibleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		MyDestructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
void ADestructibleInnerPipes::DestroyThis()
{
	MyDestructibleMesh->DestroyComponent();
//	Destroy();
}