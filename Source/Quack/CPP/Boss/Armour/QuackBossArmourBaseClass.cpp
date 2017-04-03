// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Boss/Armour/QuackBossArmourBaseClass.h"
#include "Classes/Components/DestructibleComponent.h"
#include "Classes/PhysicsEngine/RadialForceComponent.h"
#include "Headers/Misc/QuackGameMode.h"

// Sets default values
AQuackBossArmourBaseClass::AQuackBossArmourBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	ArmourPiece = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Armour"));
	ArmourPiece->SetupAttachment(MyRoot);

	DestructibleArmourPiece = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleArmourPiece"));
	DestructibleArmourPiece->SetHiddenInGame(true);
	DestructibleArmourPiece->OnComponentFracture.AddDynamic(this, &AQuackBossArmourBaseClass::OnComponentFracture);
	DestructibleArmourPiece->SetupAttachment(MyRoot);

	ArmourHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ArmourHitZone"));
	ArmourHitBox->bGenerateOverlapEvents = true;
	ArmourHitBox->SetupAttachment(ArmourPiece);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(DestructibleArmourPiece);

	bHasBeenDestroyed = false;

}

void AQuackBossArmourBaseClass::OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection)
{
	UE_LOG(LogTemp, Warning, TEXT("OnComponentFractured"));
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FTimerHandle CollisionTimer;
		World->GetTimerManager().SetTimer(CollisionTimer, this, &AQuackBossArmourBaseClass::MyDestroy, 1.5f, false);
	}
	//	FTimerHandle DestroyTimer;
	//	World->GetTimerManager().SetTimer(DestroyTimer, this, &ADestructibleInnerPipes::DestroyThis, 5.0f, false);

	//}

	//MyDestructibleMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//MyDestructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AQuackBossArmourBaseClass::MyDestroy()
{
	if(DestructibleArmourPiece != nullptr)
		DestructibleArmourPiece->DestroyComponent();
}

// Called when the game starts or when spawned
void AQuackBossArmourBaseClass::BeginPlay()
{
	Super::BeginPlay();
	if (ArmourPiece != nullptr)
	{
		ArmourPiece->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
		//ToggleHighlight(true);
	}
}

void AQuackBossArmourBaseClass::AttachToAComponent(USkeletalMeshComponent* AttachTo, FAttachmentTransformRules TransformRules, FName SocketName)
{
	if(AttachTo != nullptr)
		AttachToComponent(AttachTo, TransformRules, SocketName);
}

// Called every frame
void AQuackBossArmourBaseClass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AQuackBossArmourBaseClass::Fracture()
{
	if (bFractured) return;
	if (ArmourPiece != nullptr)
	{
		ArmourPiece->DestroyComponent();
	}
	if (DestructibleArmourPiece != nullptr && RadialForce != nullptr)
	{
		bFractured = true;
		DestructibleArmourPiece->SetHiddenInGame(false);
		DestructibleArmourPiece->ApplyRadiusDamage(ImpulseDamage, RadialForce->GetComponentLocation(), 200.0f, ImpulsePower, true);
		//RadialForce->FireImpulse();
		UE_LOG(LogTemp, Warning, TEXT("ImpulseFired"));
	}
}

void AQuackBossArmourBaseClass::Die()
{
	if (ArmourValue <= 0.0f)
	{
		bHasBeenDestroyed = true;
		ToggleHighlight(false);
		//ArmourPiece->SetHiddenInGame(true);
		ArmourHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ArmourPiece->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		UWorld* TempWorld = GetWorld();
		if (TempWorld != nullptr)
		{
			AQuackGameMode* TempGameMode = Cast<AQuackGameMode>(TempWorld->GetAuthGameMode());
			if (TempGameMode != nullptr)
			{
				float RealtimeSeconds = UGameplayStatics::GetRealTimeSeconds(TempWorld);
				FString TempString = TEXT("Armour pin destroyed at: ");
				TempString += FString::FromInt(RealtimeSeconds);
				TempString += LINE_TERMINATOR;
				TempGameMode->AddToString(TempString);
			}
		}
		// Perhaps, interpolate it out of the armour
		// Then "kill it"
	}
}

void AQuackBossArmourBaseClass::ToggleHighlight(bool _Highlight)
{
	ArmourPiece->SetRenderCustomDepth(_Highlight);
}