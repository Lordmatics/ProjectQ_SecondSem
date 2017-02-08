// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Headers/Misc/Pipe.h"
#include "TestTutorialPipe.generated.h"

/**
 * 
 */
UCLASS()
class QUACK_API ATestTutorialPipe : public APipe
{
	GENERATED_BODY()

private:

public:
	ATestTutorialPipe();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	

};
