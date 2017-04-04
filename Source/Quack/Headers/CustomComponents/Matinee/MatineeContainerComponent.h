// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MatineeContainerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UMatineeContainerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Float to use as named index for array of matinees*/
	UPROPERTY(EditAnywhere, Category = "C++ Matinee")
		int LowerPipeLeftMatinee = 0;

	/** Float to use as named index for array of matinees*/
	UPROPERTY(EditAnywhere, Category = "C++ Matinee")
		int LowerPipeRightMatinee = 1;

	/** Float to use as named index for array of matinees*/
	UPROPERTY(EditAnywhere, Category = "C++ Matinee")
		int UpperPipeMatinee = 2;

	//UPROPERTY(EditAnywhere, Category = "C++ Matinee")
	//	const float NeedleMatinee = 2;

	/** Array of Potential Matinees Used In Game*/
	UPROPERTY(EditAnywhere, Category = "C++ Matinee")
		TArray<class AMatineeActor*> Matinees;
public:	
	// Sets default values for this component's properties
	UMatineeContainerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	static float GetMatineeLength(const class AMatineeActor* MatineeInQuestion);

	float GetMatineeLengthAt(int Index);


	bool PlayLowerPipeRightMatinee();
	bool PlayLowerPipeLeftMatinee();

		
	bool PlayUpperPipeMatinee();

	bool PlayMatineeAt(int Index);

	FORCEINLINE class AMatineeActor* GetLowerPipeLeftMatinee() const { return Matinees[LowerPipeLeftMatinee]; }
	FORCEINLINE class AMatineeActor* GetLowerPipeRightMatinee() const { return Matinees[LowerPipeRightMatinee]; }
	FORCEINLINE class AMatineeActor* GetUpperPipeMatinee() const { return Matinees[UpperPipeMatinee]; }

	
};
