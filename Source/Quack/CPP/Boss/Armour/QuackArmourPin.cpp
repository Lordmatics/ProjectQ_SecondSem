// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Boss/Armour/QuackArmourPin.h"

AQuackArmourPin::AQuackArmourPin()
{
}

void AQuackArmourPin::BeginPlay()
{
	Super::BeginPlay();
	EmissivePin = ArmourPiece->CreateDynamicMaterialInstance(1);
	//UE_LOG(LogTemp, Warning, TEXT("EmissivePin should exist"));
}

void AQuackArmourPin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuackArmourPin::White()
{
	if (EmissivePin != nullptr)
	{
		FLinearColor NewColour = FLinearColor::White;
		EmissivePin->SetVectorParameterValue(FName("PinColour"), NewColour);
		UWorld* const World = GetWorld();
		if (World == nullptr) return;
		
		FTimerHandle RedHandle;
		World->GetTimerManager().SetTimer(RedHandle, this, &AQuackArmourPin::Red, TurnBackToRedDelay, false);
		//UE_LOG(LogTemp, Warning, TEXT("Pin is White"));
	}
}

void AQuackArmourPin::Red()
{
	if (EmissivePin != nullptr)
	{
		FLinearColor NewColour = FLinearColor::Red;
		EmissivePin->SetVectorParameterValue(FName("PinColour"), NewColour);
		//UE_LOG(LogTemp, Warning, TEXT("Pin is Red"));
	}
}