// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIPawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Headers/Managers/TutorialManager.h"
#include "Headers/CustomComponents/AnimationComponent.h"
#include "Headers/AIEnemies/QuackAIController.h"

// Sets default values
AQuackAIPawn::AQuackAIPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//RootComponent = SceneComponent;
	//MovementComp = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComp"));
	//MovementComp->UpdatedComponent = SceneComponent;
	//SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComp"));
	//SkeletalComp->SetupAttachment(SceneComponent);
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetCapsuleComponent());
	GetCharacterMovement()->bUseRVOAvoidance = true;

	AnimationComponent = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComponent"));
}

// Called when the game starts or when spawned
void AQuackAIPawn::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

// Called every frame
void AQuackAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AQuackAIPawn::SetupPlayerInputComponent(class UInputComponent* _InputComponent)
{
	Super::SetupPlayerInputComponent(_InputComponent);

}

void AQuackAIPawn::Attack()
{

}

void AQuackAIPawn::TakeDamages(float DamageIn)
{
	CurrentHealth -= DamageIn;
	if (CurrentHealth <= 0)
	{
		Die();
	}
}

void AQuackAIPawn::Die()
{
	if (TutorialManager != nullptr)
	{
		TutorialManager->WormDiedAt(WormID);

	}
	Destroy();
}

void AQuackAIPawn::SetBossMinion()
{
	AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
	if (TempController != nullptr)
	{
		APlayerController* TempPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (TempPlayerController != nullptr)
		{
			APawn* TempCharacter = TempPlayerController->GetControlledPawn();
			if (TempCharacter != nullptr)
			{
				TempController->Blackboard->SetValueAsObject("AttackTarget", TempCharacter);
			}
		}
	}
}