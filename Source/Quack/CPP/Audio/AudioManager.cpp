// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Audio/AudioManager.h"


// Sets default values
AAudioManager::AAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	AudioComponentA = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentA"));
	AudioComponentA->SetupAttachment(MyRoot);

	AudioComponentB = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentB"));
	AudioComponentB->SetupAttachment(MyRoot);

	EnemyRoomTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyRoomTrigger"));
	EnemyRoomTrigger->bGenerateOverlapEvents = true;
	//EnemyRoomTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAudioManager::MinionRoomEnter);
	EnemyRoomTrigger->SetupAttachment(MyRoot);
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();
	//PreviousAudioState = AudioStates::E_ExplorationLoop;
	//CurrentAudioState = AudioStates::E_ExplorationLoop;
	ChangeStateTo(AudioStates::E_ExplorationLoop, 1.5f, 1.0f);
}

// Called every frame
void AAudioManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//void AAudioManager::MinionRoomEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	ChangeStateTo(AudioStates::E_CombatLoop, 1.5f, 1.0f);
//}

void AAudioManager::HandleFadesToNewTrack(USoundWave* _Track, float Fade, float Volu)
{
	if (_Track == nullptr) return;
	if (bAudioBIsPlaying)
	{
		AudioComponentB->FadeOut(Fade, Volu);
		AudioComponentA->SetSound(_Track);
		AudioComponentA->FadeIn(Fade, Volu);
	}
	else
	{
		AudioComponentA->FadeOut(Fade, Volu);
		AudioComponentB->SetSound(_Track);
		AudioComponentB->FadeIn(Fade, Volu);
	}
}

void AAudioManager::ChangeStateTo(AudioStates _NewState, float Fade, float Volu)
{
	if (CurrentAudioState == _NewState) return;
	PreviousAudioState = CurrentAudioState;
	CurrentAudioState = _NewState;
	// If B is playing, fade out B
	// And Fade in A

	// vice versa
	switch (_NewState)
	{
		case AudioStates::E_BossIntro:
		{
			HandleFadesToNewTrack(BossIntro, Fade, Volu);
			break;
		}
		case AudioStates::E_BossLoop:
		{
			HandleFadesToNewTrack(BossLoop, Fade, Volu);
			break;
		}
		case AudioStates::E_CombatLoop:
		{
			HandleFadesToNewTrack(CombatLoop, Fade, Volu);
			break;
		}
		case AudioStates::E_EnemiesNearLoop:
		{
			HandleFadesToNewTrack(EnemiesAreNearLoop, Fade, Volu);
			break;
		}
		case AudioStates::E_ExplorationLoop:
		{
			HandleFadesToNewTrack(ExplorationLoop, Fade, Volu);
			break;
		}
		case AudioStates::E_Victory:
		{
			HandleFadesToNewTrack(BossVictory, Fade, Volu);
			break;
		}
	}
	bAudioBIsPlaying = !bAudioBIsPlaying;

}

//void AAudioManager::FadeTo(AudioStates _AudioState)
//{
//	int TrackIndex = (int)_AudioState;
//	switch (_AudioState)
//	{
//		case AudioStates::E_BossIntro:
//		{
//			break;
//		}
//		case AudioStates::E_BossLoop:
//		{
//			break;
//		}
//		case AudioStates::E_CombatLoop:
//		{
//			break;
//		}
//		case AudioStates::E_EnemiesNearLoop:
//		{
//			break;
//		}
//		case AudioStates::E_ExplorationLoop:
//		{
//			break;
//		}
//		case AudioStates::E_Victory:
//		{
//			break;
//		}
//	}
//}