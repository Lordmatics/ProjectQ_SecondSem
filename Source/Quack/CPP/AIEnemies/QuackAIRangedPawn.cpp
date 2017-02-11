// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIRangedPawn.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/Misc/QuackProjectile.h"
#include "Classes/Components/ArrowComponent.h"
#include "Headers/AIEnemies/QuackAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AQuackAIRangedPawn::AQuackAIRangedPawn()
{
	/*(PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetCapsuleComponent());
	GetCharacterMovement()->bUseRVOAvoidance = true;*/
	ProjectileSpawn = CreateDefaultSubobject<UArrowComponent>(TEXT("CodeArrowLUL"));
	ProjectileSpawn->SetupAttachment(GetCapsuleComponent());

}

void AQuackAIRangedPawn::BeginPlay()
{
	Super::BeginPlay();
	CanFire = true;
}
// Called every frame
void AQuackAIRangedPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AQuackAIRangedPawn::SetupPlayerInputComponent(class UInputComponent* _InputComponent)
{
	Super::SetupPlayerInputComponent(_InputComponent);

}

void AQuackAIRangedPawn::Attack()
{
	AQuackCharacter* PlayerCharacter = Cast<AQuackCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter != nullptr)
	{
		if (GetDistanceTo(PlayerCharacter) > AttackRange)
		{
			return;
		}
		else
			if (GetDistanceTo(PlayerCharacter) <= FleeRange)
			{
				AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
				if (TempController != nullptr)
				{
					TempController->Blackboard->SetValueAsBool("ShouldFlee", true);
				}
			}
			else
			{
				if (ProjectileSpawn != nullptr)
				{
					if (Projectile != nullptr)
					{
						UWorld* World = GetWorld();
						if (World != nullptr)
						{
							if (CanFire)
							{
								FVector Location = ProjectileSpawn->GetComponentLocation();
								FRotator Rotation = ProjectileSpawn->GetComponentRotation();
								AQuackProjectile* Proj = World->SpawnActor<AQuackProjectile>(Projectile, Location, Rotation);
								World->GetTimerManager().SetTimer(DelayTimer, this, &AQuackAIRangedPawn::ClearTimer, FireDelay, false);
								CanFire = false;
							}
						}
					}
				}
			}
	}
}

void AQuackAIRangedPawn::ClearTimer()
{
	CanFire = true;
}
