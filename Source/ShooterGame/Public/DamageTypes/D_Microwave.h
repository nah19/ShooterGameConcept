// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageTypes/ConceptDamageType.h"
#include "D_Microwave.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UD_Microwave : public UConceptDamageType
{
	GENERATED_BODY()

public:
	UD_Microwave();
	void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);
	
	float DisruptTime = 3.0f;
};
