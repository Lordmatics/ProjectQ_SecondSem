// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/BossAttacksComponent.h"
#include "Headers/Misc/QuackProjectile.h"
#include "Headers/Misc/ChildProjectiles/BossBileProjectile.h"
#include "Classes/Animation/AnimInstance.h"
#include "Headers/CustomComponents/AnimationComponent.h"
#include "Classes/Components/ArrowComponent.h"

// Sets default values for this component's properties
UBossAttacksComponent::UBossAttacksComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossAttacksComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}


// Called every frame
void UBossAttacksComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

// BILE SPIT 
void UBossAttacksComponent::StartBileSpitting(UArrowComponent* MouthArrow, UAnimInstance* BossAnimInstance, UAnimationComponent* BossAnimComponent, float OverridenFireRate)
{
	if (bIsBileSpitting) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		float Rate = 1.0f / BileFireRate;
		if (OverridenFireRate < 1.0f)
		{
			Rate = 1.0f / OverridenFireRate;
		}
		//ShootTheBile(MouthArrow, BossAnimInstance, BossAnimComponent);
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("BeginShootTheBile"), MouthArrow, BossAnimInstance, BossAnimComponent);
		World->GetTimerManager().SetTimer(BileTimer, TimerDel, Rate, true);
		//World->GetTimerManager().SetTimer(BileTimer, this, &UBossAttacksComponent::ShootTheBile, Rate, true);
		bIsBileSpitting = true;
		UE_LOG(LogTemp, Warning, TEXT("UBossAttacksComponent: StartBileSpitting: Rate: %f"), Rate);
		//bWasBileMostRecent = true;
	}
}

void UBossAttacksComponent::BeginShootTheBile(UArrowComponent* MouthArrow, UAnimInstance* BossAnimInstance, UAnimationComponent* BossAnimComponent)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (BossAnimComponent != nullptr && BossAnimInstance != nullptr)
	{
		if (BossAnimComponent->GetBileSpitAnim() == nullptr) return;
		float BileAnimDuration = BossAnimInstance->Montage_Play(BossAnimComponent->GetBileSpitAnim(), 1.0f);
		FTimerHandle AnimDelayHandle;
		FTimerDelegate AnimDelayDelegate;
		AnimDelayDelegate.BindUFunction(this, FName("ShootTheBile"), MouthArrow, BossAnimInstance, BossAnimComponent);
		World->GetTimerManager().SetTimer(AnimDelayHandle, AnimDelayDelegate, 9 * (BileAnimDuration / 10), false);
	}
}

void UBossAttacksComponent::ShootTheBile(UArrowComponent* MouthArrow, UAnimInstance* BossAnimInstance, UAnimationComponent* BossAnimComponent)
{
	// PLAY ANIM MONTAGE BEFORE THIS FUNCTION IN BOSS CONTAINER

	// FIRES THE PROJECTILE
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (BossProjectilesArray.Num() == 0) return;
	if (BossProjectilesArray[1] == nullptr) return;

	//CurrentAnimationState = AnimationStates::E_AnimBileSpit;
	if (MouthArrow == nullptr) return;
	const FVector Location = MouthArrow->GetComponentLocation();
	const FRotator Rotation = MouthArrow->GetComponentRotation();
	AQuackProjectile* Proj = World->SpawnActor<AQuackProjectile>(BossProjectilesArray[1], Location, Rotation);
	if (Proj != nullptr)
	{
		float NewScale = 0.5f; // 0.5f * BileShotsFired;

		// THIS PROBABLY WANTS CHANGING TO ADJUST TO NEW ENVIRONMENTS FLOORS
		// DO THIS SOME OTHER TIME THOUGH
		BileShotsFired++;
		if (BileShotsFired > 3)
			BileShotsFired = 1;
			//med far close
		if (BileShotsFired == 1)
			NewScale = 0.375f;
		else if (BileShotsFired == 2)
			NewScale = 0.075f;
		else if (BileShotsFired == 3)
			NewScale = 0.675f;
		Proj->AdjustProjectileMovementScale(NewScale);
	}
		
	
}

void UBossAttacksComponent::StopBileSpitting()
{
	bIsBileSpitting = false;
	//bWasBileMostRecent = false;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(BileTimer);
	}
}

// END BILE SPIT

// TAIL SHOT
void UBossAttacksComponent::StartTailShooting(class UArrowComponent* TailArrow, class UAnimInstance* BossAnimInstance, class UAnimationComponent* BossAnimComponent, float OverridenFireRate)
{
	if (bIsTailShooting) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		float Rate = TailFireRate;
		if (OverridenFireRate != TailFireRate)
		{
			Rate = OverridenFireRate;
		}
		//ShootTheBile(MouthArrow, BossAnimInstance, BossAnimComponent);
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("BeginTailShoot"), TailArrow, BossAnimInstance, BossAnimComponent);
		World->GetTimerManager().SetTimer(TailTimer, TimerDel, Rate, true);
		bIsTailShooting = true;
		UE_LOG(LogTemp, Warning, TEXT("UBossAttacksComponent: StartTailShooting: Rate: %f"), Rate);
	}
}

void UBossAttacksComponent::BeginTailShoot(UArrowComponent* TailArrow, UAnimInstance* BossAnimInstance, UAnimationComponent* BossAnimComponent)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (BossAnimComponent != nullptr && BossAnimInstance != nullptr)
	{
		if (BossAnimComponent->GetTailShootAnim() == nullptr) return;
		float TailAnimDuration = BossAnimInstance->Montage_Play(BossAnimComponent->GetTailShootAnim(), 1.0f);
		FTimerHandle AnimDelayHandle;
		FTimerDelegate AnimDelayDelegate;
		AnimDelayDelegate.BindUFunction(this, FName("TailShoot"), TailArrow, BossAnimInstance, BossAnimComponent);
		World->GetTimerManager().SetTimer(AnimDelayHandle, AnimDelayDelegate, TailAnimDuration, false);
	}
}

void UBossAttacksComponent::TailShoot(class UArrowComponent* TailArrow, class UAnimInstance* BossAnimInstance, class UAnimationComponent* BossAnimComponent)
{
	// FIRES THE PROJECTILE
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (BossProjectilesArray.Num() == 0) return;
	if (TailArrow != nullptr)
	{
		if (BossProjectilesArray[0] != nullptr)
		{
			const FVector Location = TailArrow->GetComponentLocation();
			const FRotator Rotation = TailArrow->GetComponentRotation();
			World->SpawnActor<AQuackProjectile>(BossProjectilesArray[0], Location, Rotation);		
		}
	}
}

void UBossAttacksComponent::StopTailShoot()
{
	bIsTailShooting = false;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(TailTimer);
	}
}
// END TAIL SHOT
