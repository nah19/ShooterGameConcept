// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ConceptDamageBehavior.h"
#include "ConceptCharacter.h"
#include "DB_Character.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UDB_Character : public UConceptDamageBehavior
{
	GENERATED_BODY()
	
	
public:
	AConceptCharacter* MyCharacter;

	UDB_Character();

	UPROPERTY()
		UForceFeedbackEffect* DamageForceFeedback;

	void Initialize(AActor* ActorToApplybehavior) override;

	void ApplyLifeDamage(float Damage, TSubclassOf<UDamageType> DamageType = NULL) override;

	void ApplyLifeRecovery(float Recover) override;

	void ApplyPoisonDamage(AController* KillerPlayer, float Damage, float poisonStatusTimer, float poisonDamageTimer) override;

	void ApplyConstrictionDamage(AController* KillerPlayer, float Damage, float poisonStatusTimer, float poisonDamageTimer) override;

	void ApplyHurtShout(float Damage) override;

	void ApplyBurnDamage(AController* KillerPlayer, float Damage) override;

	void ApplyDisruptDamage(float DisruptTime) override;
    void ApplyStunDamage(float StunTime) override;
    void ApplySnareDamage(float SnareTime) override;
    void ApplySlowDamage(float SlowTime, float SlowAmount) override;


	void ZeroGravityStatus(float Time) override;

	bool IsPawnDied() override;

	void ApplyRadialImpulse(AController* KillerPlayer, FVector Location, float Radius, float Damage, float TimeToStop)  override;

	virtual void ApplyHitAnim(float Time, float Damage) override;

    void Killed(AConceptGameMode* MyGameMode, AController* KillerPlayer, AController* KilledPlayer, FAbilityData AbilityData) override;


	
};
