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

	UPROPERTY(EditAnywhere, Category = "C++ Boss Animations")
		UAnimMontage* MeleeLeft;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Animations")
		UAnimMontage* MeleeRight;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Animations")
		UAnimMontage* MeleeBoth;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Animations")
		UAnimMontage* TailShoot;

	UPROPERTY(EditAnywhere, Category = "C++ Boss Animations")
		UAnimMontage* BileSpit;
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

	// Boss Attack Animations
	FORCEINLINE UAnimMontage* GetMeleeAnimLeft() const { return MeleeLeft; }
	FORCEINLINE UAnimMontage* GetMeleeAnimRight() const { return MeleeRight; }
	FORCEINLINE UAnimMontage* GetMeleeAnimBoth() const { return MeleeBoth; }
	FORCEINLINE UAnimMontage* GetTailShootAnim() const { return TailShoot; }
	FORCEINLINE UAnimMontage* GetBileSpitAnim() const { return BileSpit; }	
};
