// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ConceptDamageBehavior.h"
#include "MinionCharacter.h"
#include "DB_Minion.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UDB_Minion : public UConceptDamageBehavior
{
	GENERATED_BODY()

public:
	//ASpeeder* MyMinion;
	AMinionCharacter* MyMinion;

	UDB_Minion();

	void Initialize(AActor* ActorToApplybehavior) override;

	void ApplyLifeDamage(float Damage, TSubclassOf<UDamageType> DamageType = NULL) override;

	bool IsPawnDied() override;

	void Killed(AConceptGameMode* MyGameMode, AController* KillerPlayer, AController* KilledPlayer, FAbilityData AbilityData) override;

};
