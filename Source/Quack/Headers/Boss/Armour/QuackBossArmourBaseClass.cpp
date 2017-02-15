// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Boss/Armour/QuackBossArmourBaseClass.h"


// Sets default values
AQuackBossArmourBaseClass::AQuackBossArmourBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	ArmourHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ArmourHitZone"));
	ArmourHitBox->bGenerateOverlapEvents = true;
	ArmourHitBox->SetupAttachment(MyRoot);

	ArmourPiece = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Armour"));
	ArmourPiece->SetupAttachment(ArmourHitBox);

	bHasBeenDestroyed = false;
}

// Called when the game starts or when spawned
void AQuackBossArmourBaseClass::BeginPlay()
{
	Super::BeginPlay();
	if (ArmourPiece != nullptr)
	{
		ArmourPiece->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
		ToggleHighlight(true);
	}
}

// Called every frame
void AQuackBossArmourBaseClass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AQuackBossArmourBaseClass::Die()
{
	if (ArmourValue <= 0.0f)
	{
		bHasBeenDestroyed = true;
		ToggleHighlight(false);
		// Perhaps, interpolate it out of the armour
		// Then "kill it"
	}
}

void AQuackBossArmourBaseClass::ToggleHighlight(bool _Highlight)
{
	ArmourPiece->SetRenderCustomDepth(_Highlight);
}