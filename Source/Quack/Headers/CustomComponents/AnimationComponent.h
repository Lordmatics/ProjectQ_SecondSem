// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "AnimationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUACK_API UAnimationComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* AttackAnim_A;

	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* AttackAnim_B;

	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* AttackAnim_C;

	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* DeathAnim;
public:	
	// Sets default values for this component's properties
	UAnimationComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Forceinlines
	FORCEINLINE UAnimMontage* GetAttackAnimMontageA() const { return AttackAnim_A; }
	FORCEINLINE UAnimMontage* GetAttackAnimMontageB() const { return AttackAnim_B; }
	FORCEINLINE UAnimMontage* GetAttackAnimMontageC() const { return AttackAnim_C; }

	FORCEINLINE UAnimMontage* GetDeathAnimMontage() const { return DeathAnim; }
	
};
