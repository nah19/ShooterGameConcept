// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "GameFramework/Actor.h"
#include "StaticFunctionLibrary.h"
#include "ConceptDamageBehavior.generated.h"


class AConceptGameMode;

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UConceptDamageBehavior : public UObject
{
	GENERATED_BODY()
	
public: 

	UConceptDamageBehavior();

	virtual void Initialize(AActor* ActorToApplybehavior);

	virtual void ApplyLifeDamage(float Damage, TSubclassOf<UDamageType> DamageType = NULL);

	virtual void ApplyLifeRecovery(float Recover);

	virtual void ApplyPoisonDamage(AController* KillerPlayer, float Damage, float poisonStatusTimer, float poisonDamageTimer);

	virtual void ApplyConstrictionDamage(AController* KillerPlayer, float Damage, float constrictionStatusTimeLeft, float constrictionDamageTimeLeft);

	virtual void ApplyRadialImpulse(AController* KillerPlayer, FVector Location, float Radius, float Damage, float TimeToStop);

	virtual void ApplyHitAnim(float Time, float Damage);

	virtual void ApplyBurnDamage(AController* KillerPlayer, float Damage);

	virtual void ApplyDisruptDamage(float DisruptTime);
    virtual void ApplyStunDamage(float StunTime);
    virtual void ApplySnareDamage(float SnareTime);
    virtual void ApplySlowDamage(float SlowTime, float SlowAmount);

	virtual void ZeroGravityStatus(float Time);

	virtual bool IsPawnDied();

	virtual void Killed(AConceptGameMode* MyGameMode, AController* KillerPlayer, AController* KilledPlayer, FAbilityData AbilityData);

	virtual void ApplyHurtShout(float Damage);
};
