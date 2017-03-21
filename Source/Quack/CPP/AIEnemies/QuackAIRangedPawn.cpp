// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIRangedPawn.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/Misc/QuackProjectile.h"
#include "Classes/Components/ArrowComponent.h"
#include "Headers/AIEnemies/QuackAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Headers/Misc/ImpactEffects/BileSplashEffect.h"

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
						UWorld* const World = GetWorld();
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

void AQuackAIRangedPawn::TakeDamages(float DamageIn)
{
	CurrentHealth -= DamageIn;
	if (CurrentHealth <= 0)
	{
		if (OnEnemyDestroyed.IsBound())
			OnEnemyDestroyed.Broadcast(this);
		if (OnEnemyDestroyedRoom.IsBound())
			OnEnemyDestroyedRoom.Broadcast(this);
		UWorld* TempWorld = GetWorld();
		BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		if (TempWorld != nullptr && DeathParticle != nullptr)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			TempWorld->SpawnActor<ABileSplashEffect>(DeathParticle, GetActorLocation(), GetActorRotation(), SpawnParams);
		}
		//UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
		Destroy();
	}
}