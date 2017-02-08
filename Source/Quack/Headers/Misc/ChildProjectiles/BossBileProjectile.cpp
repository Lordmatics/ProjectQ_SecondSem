// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Misc/ChildProjectiles/BossBileProjectile.h"
#include "Headers/Character/QuackCharacter.h"
#include "Headers/Misc/ImpactEffects/BileSplashEffect.h"

ABossBileProjectile::ABossBileProjectile()
{

}

void ABossBileProjectile::BeginPlay()
{
	Super::BeginPlay();
	//RandomiseProjection();
}

void ABossBileProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossBileProjectile::RandomiseProjection()
{
	float Rand = FMath::RandRange(RandMinVelScale, RandMaxVelScale);
	Scalar = Rand;
	AdjustProjectileMovementScale(Scalar);

	//ProjectileMovement->InitialSpeed = Rand;
	//ProjectileMovement->MaxSpeed = Rand;
}

void ABossBileProjectile::SomethingHit(AActor* OtherActor)
{
	Super::SomethingHit(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("%s, was hit CHILD CLASS - Bile SHOT"), *(OtherActor->GetName()));
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		MyCharacter->DecreaseHealth(ProjectileDamage);
		//SpawnBile(MyCharacter->GetActorLocation(), FRotator::ZeroRotator);//OtherActor->GetActorRotation());
		if (CollisionComp != nullptr)
			SpawnBile(CollisionComp->GetComponentLocation() - FVector(0.0f,0.0f,150.0f), FRotator::ZeroRotator);//OtherActor->GetActorRotation());

		Destroy();
	}
	else
	{
		//if(OtherActor != nullptr)
		if(CollisionComp != nullptr)
			SpawnBile(CollisionComp->GetComponentLocation(), FRotator::ZeroRotator);//OtherActor->GetActorRotation());
		Destroy();
	}
}

void ABossBileProjectile::SpawnBile(const FVector& HitLocation, const FRotator& HitRotation)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (BileSplashClass != nullptr)
	{
		ABileSplashEffect* Bile = World->SpawnActor<ABileSplashEffect>(BileSplashClass, HitLocation, HitRotation);
		if (Bile != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Success Bile splash spawn"));
		}
	}

}