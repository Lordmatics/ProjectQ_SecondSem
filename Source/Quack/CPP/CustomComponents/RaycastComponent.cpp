// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/CustomComponents/RaycastComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
URaycastComponent::URaycastComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URaycastComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URaycastComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

FHitResult URaycastComponent::Raycast(UCameraComponent* FirstPersonCamera, const TArray<TWeakObjectPtr<AActor>>& IgnoredActors)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return FHitResult();
	FHitResult Hit;
	FVector Start = FirstPersonCamera->GetComponentLocation();
	FVector Forward = FirstPersonCamera->GetForwardVector();
	FVector End = Start + (Forward * Raylength);
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActors(IgnoredActors);
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CQP))
	{
		CreateBulletHole(Hit);
		return Hit;
	}
	else return FHitResult();
}

// ForLaser
FHitResult URaycastComponent::RaycastLaser(UParticleSystemComponent* ParticleSystem, const TArray<TWeakObjectPtr<AActor>>& IgnoredActors)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return FHitResult();
	FHitResult Hit;
	FVector Start = ParticleSystem->GetComponentLocation();
	FVector Forward = ParticleSystem->GetForwardVector();
	FVector End = Start + (Forward * Raylength);
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActors(IgnoredActors);
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CQP))
	{
		return Hit;
	}
	else return FHitResult();
}

FHitResult URaycastComponent::RaycastBossLaser(UParticleSystemComponent* ParticleSystem,const FVector& EndLocation, const TArray<TWeakObjectPtr<AActor>>& IgnoredActors)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return FHitResult();
	FHitResult Hit;
	FVector Start = ParticleSystem->GetComponentLocation();
	//FVector Forward = ParticleSystem->GetForwardVector();
	//FVector End = Start + (Forward * Raylength);
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActors(IgnoredActors);
	//DrawDebugLine(World, Start, EndLocation, FColor::Red, true);

	if (World->LineTraceSingleByChannel(Hit, Start, EndLocation, ECC_Visibility, CQP))
	{
		return Hit;
	}
	else return FHitResult();
}

void URaycastComponent::CreateBulletHole(FHitResult HitResult)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (BulletHoleDecal != nullptr)
	{
		FVector BulletHoleSizxe = DecalSize;
		UGameplayStatics::SpawnDecalAtLocation(World, BulletHoleDecal, BulletHoleSizxe, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), BulletDecalLifetime);
	}
}

FHitResult URaycastComponent::GetDownHit(AActor* _Actor, const TArray<TWeakObjectPtr<AActor>>& IgnoredActors)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return FHitResult();
	FHitResult Hit;
	FVector Start = _Actor->GetActorLocation();
	//FVector Forward = ParticleSystem->GetForwardVector();
	//FVector End = Start + (Forward * Raylength);
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActors(IgnoredActors);
	//DrawDebugLine(World, Start, EndLocation, FColor::Red, true);
	const FVector EndLocation = Start -(FVector::UpVector * 1000.0f);
	if (World->LineTraceSingleByChannel(Hit, Start, EndLocation, ECC_Visibility, CQP))
	{
		return Hit;
	}
	else return FHitResult();
}