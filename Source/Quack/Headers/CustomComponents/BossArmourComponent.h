// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BossArmourComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UBossArmourComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
	//	float ArmourUR = 100.0f;
	//UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
	//	float ArmourUL = 100.0f;
	//UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
	//	float ArmourLR = 100.0f;
	//UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
	//	float ArmourLL = 100.0f;
public:	
	// Sets default values for this component's properties
	UBossArmourComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
		TSubclassOf<class AQuackBossArmourBaseClass> MainBodyArmourClass;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
		TSubclassOf<class AQuackBossArmourBaseClass> PinClassUR;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
		TSubclassOf<class AQuackBossArmourBaseClass> PinClassUL;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
		TSubclassOf<class AQuackBossArmourBaseClass> PinClassLR;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Armour")
		TSubclassOf<class AQuackBossArmourBaseClass> PinClassLL;
	// FORCEINLINES
	//FORCEINLINE float GetArmourUR() const { return ArmourUR; }
	//FORCEINLINE float GetArmourUL() const { return ArmourUL; }
	//FORCEINLINE float GetArmourLR() const { return ArmourLR; }
	//FORCEINLINE float GetArmourLL() const { return ArmourLL; }
	
};
