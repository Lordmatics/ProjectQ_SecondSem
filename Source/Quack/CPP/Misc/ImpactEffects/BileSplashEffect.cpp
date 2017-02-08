// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/ImpactEffects/BileSplashEffect.h"
#include "Headers/Character/QuackCharacter.h"

// Sets default values
ABileSplashEffect::ABileSplashEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	TEMPBile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TEMPMesh"));
	TEMPBile->bGenerateOverlapEvents = true;
	TEMPBile->OnComponentBeginOverlap.AddDynamic(this, &ABileSplashEffect::OnTriggerEnter);
	TEMPBile->OnComponentEndOverlap.AddDynamic(this, &ABileSplashEffect::OnTriggerExit);
	TEMPBile->SetupAttachment(MyRoot);
}

// Called when the game starts or when spawned
void ABileSplashEffect::BeginPlay()
{
	Super::BeginPlay();
	StartDOT();
}

// Called every frame
void ABileSplashEffect::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABileSplashEffect::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("BILETRIGGEREXIT"));
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		CharRef = nullptr;
		ClearDOT();
	}
}

void ABileSplashEffect::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("BILETRIGGERENTER"));
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr && CharRef == nullptr)
	{
		CharRef = MyCharacter;
		// Reset timer to activate upon re-enterring
		ClearDOT();
		StartDOT();
	}
}

void ABileSplashEffect::StartDOT()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	DOTDamage();
	World->GetTimerManager().SetTimer(BileTimerDOT, this, &ABileSplashEffect::DOTDamage, DOTFrequency, true);
}

void ABileSplashEffect::ClearDOT()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().ClearTimer(BileTimerDOT);
}

void ABileSplashEffect::DOTDamage()
{
	if (CharRef == nullptr) return;
	CharRef->DecreaseHealth(BileDamageOverTime);
}