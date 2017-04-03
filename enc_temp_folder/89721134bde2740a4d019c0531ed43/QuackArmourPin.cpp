// Fill out your copyright notice in the Description page of Project Settings.

#include "Headers/Quack.h"
#include "Headers/Boss/Armour/QuackArmourPin.h"

AQuackArmourPin::AQuackArmourPin()
{
	bShouldFadeIn = false;
}

void AQuackArmourPin::BeginPlay()
{
	Super::BeginPlay();
	PinMaterial = ArmourPiece->CreateDynamicMaterialInstance(0);
	EmissivePin = ArmourPiece->CreateDynamicMaterialInstance(1);


	//UE_LOG(LogTemp, Warning, TEXT("EmissivePin should exist"));
}

void AQuackArmourPin::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PinMaterial != nullptr && EmissivePin != nullptr)
	{
		PinMaterial->SetScalarParameterValue(FName("Dissolve"), Alpha);
		EmissivePin->SetScalarParameterValue(FName("Dissolve"), Alpha);
	}
}

void AQuackArmourPin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If Should be Dissolving, deduct transparency && Pin has no HP
	if (bHasBeenDestroyed)
	{
		if (PinMaterial != nullptr && EmissivePin != nullptr)
		{
			Alpha -= DeltaTime * DissolveRate;
			Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("AlphaDecreasing: %f"), Alpha);
			PinMaterial->SetScalarParameterValue(FName("Dissolve"), Alpha);
			EmissivePin->SetScalarParameterValue(FName("Dissolve"), Alpha);
			//// If Transparency hits 0, make it hidden
			//if (Alpha <= 0.0f)
			//{
			//	ArmourPiece->SetHiddenInGame(true);
			//}
		}
	}
	// If Pin has HP and starts invisible, fade it in
	else
	{
		if (PinMaterial != nullptr && EmissivePin != nullptr && bShouldFadeIn)
		{
			// If Want Pins to fade in
			Alpha += DeltaTime * DissolveRate;
			Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("AlphaIncreasing: %f"), Alpha);
			PinMaterial->SetScalarParameterValue(FName("Dissolve"), Alpha);
			EmissivePin->SetScalarParameterValue(FName("Dissolve"), Alpha);
		}
	}
}

void AQuackArmourPin::SetAlpha(float _Alpha)
{
	Alpha = _Alpha;
	UE_LOG(LogTemp, Warning, TEXT("AlphaSet"));
	Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
	FTimerHandle TempHandle;
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().SetTimer(TempHandle, this, &AQuackArmourPin::UpdateDissolve, Delay, false);

}

void AQuackArmourPin::UpdateDissolve()
{
	if (PinMaterial != nullptr && EmissivePin != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pins Exist"));
		PinMaterial->SetScalarParameterValue(FName("Dissolve"), Alpha);
		EmissivePin->SetScalarParameterValue(FName("Dissolve"), Alpha);
	}
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