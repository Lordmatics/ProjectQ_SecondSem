// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Headers/Quack.h"
#include "Headers/Misc/QuackProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Headers/Character/QuackCharacter.h"

AQuackProjectile::AQuackProjectile() 
{
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyProjRoot"));
	RootComponent = MyRoot;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AQuackProjectile::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AQuackProjectile::OnTriggerEnter);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetupAttachment(MyRoot);
	// Set as root component
	//RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = InitialVel;
	ProjectileMovement->MaxSpeed = MaxVel;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = bCanBounce;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AQuackProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->bShouldBounce = bCanBounce;
	AdjustProjectileMovement(InitialVel, MaxVel);


	//ProjectileMovement->InitialSpeed = InitialVel;
	//ProjectileMovement->MaxSpeed = MaxVel;
}

void AQuackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuackProjectile::AdjustProjectileMovement(float _InitSpd, float _MaxVel)
{
	ProjectileMovement->InitialSpeed = _InitSpd;
	ProjectileMovement->MaxSpeed = _MaxVel;
}

void AQuackProjectile::AdjustProjectileMovementScale(float _RandomVel)
{
	ProjectileMovement->ProjectileGravityScale = _RandomVel;
	//FVector(ProjectileMovement->Velocity.X * _RandomVel, ProjectileMovement->Velocity.Y *  _RandomVel, ProjectileMovement->Velocity.Z * _RandomVel);
}

void AQuackProjectile::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SomethingHit(OtherActor);
}

void AQuackProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SomethingHit(OtherActor);

	//AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	//if (MyCharacter != nullptr)
	//{
	//	MyCharacter->DecreaseHealth(ProjectileDamage);
	//	Destroy();
	//}
	//else
	//{
	//	Destroy();
	//}
	// Only add impulse and destroy projectile if we hit a physics
	//if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	//{
	//	OtherComp->AddImpulseAtLocation(GetVelocity() * GravityForce, GetActorLocation());

	//	Destroy();
	//}
}

void AQuackProjectile::SomethingHit(AActor* OtherActor)
{
	// Overriden by base class implementations

	//UE_LOG(LogTemp,Warning,TEXT("Something Was Hit BASE CLASS"))
	//AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	//if (MyCharacter != nullptr)
	//{
	//	MyCharacter->DecreaseHealth(ProjectileDamage);
	//	Destroy();
	//}
	//else
	//{
	//	Destroy();
	//}
}