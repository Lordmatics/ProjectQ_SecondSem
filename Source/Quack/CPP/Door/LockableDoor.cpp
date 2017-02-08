// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "LockableDoor.h"
#include "Headers/Character/QuackCharacter.h"

// Sets default values
ALockableDoor::ALockableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(MyRoot);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(MyRoot);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(MyRoot);

	BoxEnter = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxEnter"));
	BoxEnter->bGenerateOverlapEvents = true;
	BoxEnter->OnComponentBeginOverlap.AddDynamic(this, &ALockableDoor::OnEnterStart);
	BoxEnter->OnComponentEndOverlap.AddDynamic(this, &ALockableDoor::OnEnterEnd);
	BoxEnter->SetupAttachment(MyRoot);

	BoxExit = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxExit"));
	BoxExit->bGenerateOverlapEvents = true;
	//BoxExit->OnComponentBeginOverlap.AddDynamic(this, &ALockableDoor::OnEnterStart);
	BoxExit->OnComponentBeginOverlap.AddDynamic(this, &ALockableDoor::OnExitStart);
	BoxExit->SetupAttachment(MyRoot);
}

// Called when the game starts or when spawned
void ALockableDoor::BeginPlay()
{
	Super::BeginPlay();
	LeftStartLocation = LeftDoor->GetRelativeTransform().GetLocation();
	RightStartLocation = RightDoor->GetRelativeTransform().GetLocation();

	if (bLocked)
	{
		if (LockedMat != nullptr)
		{
			LeftDoor->SetMaterial(0, LockedMat);
			RightDoor->SetMaterial(0, LockedMat);
		}
	}
	else
	{
		if (UnlockedMat != nullptr)
		{
			LeftDoor->SetMaterial(0, UnlockedMat);
			RightDoor->SetMaterial(0, UnlockedMat);
		}
	}
	
}

// Called every frame
void ALockableDoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


	if (bIsOpen)
	{
		if (bLocked) return;
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

// First Trigger
void ALockableDoor::OnEnterStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtheComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bLocked) return;
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("EnterredFront"));
		bIsOpen = true;
	}
}

void ALockableDoor::OnEnterEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtheComp, int32 OtherBodyIndex)
{
	//if (bLocked) return;
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ExitedFront"));
		bIsOpen = false;

	}
}

// Second Trigger
void ALockableDoor::OnExitStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtheComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuackCharacter* MyCharacter = Cast<AQuackCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("EnteredBack"));
		CloseAndLock();
		//if (bLockable)
		//{
		//	bLocked = true;
		//	if (LockedMat != nullptr)
		//	{
		//		LeftDoor->SetMaterial(0, LockedMat);
		//		RightDoor->SetMaterial(0, LockedMat);
		//	}
		//	bIsOpen = false;
		//}
	}
}

void ALockableDoor::OpenAndUnlock(bool _OpenToo = true)
{
	if(bLockable)
		bLocked = false;
	if (UnlockedMat != nullptr)
	{
		LeftDoor->SetMaterial(0, UnlockedMat);
		RightDoor->SetMaterial(0, UnlockedMat);
	}
	if(_OpenToo)
		bIsOpen = true;
}

void ALockableDoor::CloseAndLock()
{
	if (bLockable)
	{
		bLocked = true;
		if (LockedMat != nullptr)
		{
			LeftDoor->SetMaterial(0, LockedMat);
			RightDoor->SetMaterial(0, LockedMat);
		}
		bIsOpen = false;
	}
}

void ALockableDoor::OpenDoor(float DeltaTime)
{
	// Feel you have more control over the smoothness with this approach
	FVector L = LeftDoor->GetRelativeTransform().GetLocation();
	L = FMath::VInterpConstantTo(L, LeftStartLocation + FVector(-OpenLength, 0.0f, 0.0f), DeltaTime, OpenSpeed);
	LeftDoor->SetRelativeLocation(L);

	FVector R = RightDoor->GetRelativeTransform().GetLocation();
	R = FMath::VInterpConstantTo(R, RightStartLocation + FVector(OpenLength, 0.0f, 0.0f), DeltaTime, OpenSpeed);
	RightDoor->SetRelativeLocation(R);

	//LeftDoor->SetRelativeLocation(FMath::Lerp(L, FVector(-200, 0, 0), speed));

	//RightDoor->SetRelativeLocation(FMath::Lerp(R, FVector(200, 0, 0), speed));
}

void ALockableDoor::CloseDoor(float DeltaTime)
{
	FVector L = LeftDoor->GetRelativeTransform().GetLocation();
	L = FMath::VInterpConstantTo(L, LeftStartLocation, DeltaTime, OpenSpeed);
	LeftDoor->SetRelativeLocation(L);

	FVector R = RightDoor->GetRelativeTransform().GetLocation();
	R = FMath::VInterpConstantTo(R, RightStartLocation, DeltaTime, OpenSpeed);
	RightDoor->SetRelativeLocation(R);
}