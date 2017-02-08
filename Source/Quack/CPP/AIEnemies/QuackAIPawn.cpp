// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/AIEnemies/QuackAIPawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/DestructibleComponent.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/AIEnemies/QuackAIController.h"
#include "Headers/Managers/TutorialManager.h"

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
	ExplodingMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	ExplodingMesh->OnComponentFracture.AddDynamic(this, &AQuackAIPawn::OnComponentFracture);
	ExplodingMesh->SetupAttachment(GetCapsuleComponent());
	GetCharacterMovement()->bUseRVOAvoidance = true;
}

// Called when the game starts or when spawned
void AQuackAIPawn::BeginPlay()
{
	Super::BeginPlay();
	ExplodingMesh->SetVisibility(false);
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
	UE_LOG(LogTemp, Warning, TEXT("Get Attacked"));
	AQuackCharacter* PlayerCharacter = Cast<AQuackCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter != nullptr)
	{
		//PlayerCharacter->GetCharacterMovement()->AddImpulse(GetActorLocation() - PlayerCharacter->GetActorLocation());
		PlayerCharacter->DecreaseHealth(Damage);
		GetMesh()->SetVisibility(false);
		//do attack stuff
		ExplodingMesh->SetVisibility(true);
		ExplodingMesh->ApplyRadiusDamage(100, ExplodingMesh->GetComponentLocation(), 20, 10000, true);
		if (TutorialManager != nullptr)
		{
			TutorialManager->WormDiedAt(WormID);
		}
		AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
		if (TempController != nullptr)
		{
			TempController->Blackboard->SetValueAsBool("Dead", true);
			//TempController->GetBrainComponent()->StopLogic(TEXT("Dead"));
		}
		//Destroy();	
	}
}

void AQuackAIPawn::OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection)
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ExplodingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AQuackAIPawn::DestroyThis, 2.0f, false);
	if(OnEnemyDestroyed.IsBound())
		OnEnemyDestroyed.Broadcast(this);
	if (OnEnemyDestroyedRoom.IsBound())
		OnEnemyDestroyedRoom.Broadcast(this);
}

void AQuackAIPawn::DestroyThis()
{
	Destroy();
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
	GetMesh()->SetVisibility(false);
	ExplodingMesh->SetVisibility(true);
	ExplodingMesh->ApplyRadiusDamage(100, ExplodingMesh->GetComponentLocation(), 20, 10000, true);
	AQuackAIController* TempController = Cast<AQuackAIController>(GetController());
	if (TempController != nullptr)
	{
		//TempController->GetBrainComponent()->StopLogic(TEXT("Dead"));
		TempController->Blackboard->SetValueAsBool("Dead", true);
	}
}