// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Audio/AudioZoneBox.h"
#include "Headers/Audio/AudioManager.h"
#include "Headers/Character/QuackCharacter.h"
#include "EngineUtils.h"

// Sets default values
AAudioZoneBox::AAudioZoneBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	EnemyRoomTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyRoomTrigger"));
	EnemyRoomTrigger->bGenerateOverlapEvents = true;
	EnemyRoomTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAudioZoneBox::MinionRoomEnter);
	EnemyRoomTrigger->SetupAttachment(MyRoot);


}

// Called when the game starts or when spawned
void AAudioZoneBox::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AAudioManager> ActorItr(World); ActorItr; ++ActorItr)
		{
			AudioManager = *ActorItr;
		}
	}

}

// Called every frame
void AAudioZoneBox::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AAudioZoneBox::MinionRoomEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		if (AudioManager != nullptr)
		{
			if (bCanBeLocked)
			{
				if (bIsCombatTrigger)
				{
					AudioManager->ChangeStateTo(StateToTransitionTo, FadeInDuration, NewTrackVolume);
					bIsCombatTrigger = false;
				}
			}
			else
			{
				AudioManager->ChangeStateTo(StateToTransitionTo, FadeInDuration, NewTrackVolume);
			}

			if (bIsBossTrigger)
			{
				MyCharacter->bEnteredBossRoom = true;
				UWorld* const World = GetWorld();
				if (World != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Music Changed"));
					FTimerHandle BossLoopTimer;
					FTimerDelegate TimerDel;
					TimerDel.BindUFunction(this, FName("PlayBossLoop"), FadeInDuration, NewTrackVolume);
					World->GetTimerManager().SetTimer(BossLoopTimer, TimerDel, BossIntroLength, false);
					bIsBossTrigger = false;
				}
			}
		}
	}
}

void AAudioZoneBox::PlayBossLoop(float Fade, float Volu)
{
	if (AudioManager != nullptr)
	{
		AudioManager->ChangeStateTo(AudioStates::E_BossLoop, Fade, Volu);
	}
}