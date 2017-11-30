// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageTypes/ConceptDamageType.h"
#include "D_DeathBlowPrimary.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UD_DeathBlowPrimary : public UConceptDamageType
{
	GENERATED_BODY()
	
	
	class UClass* CamShake;
	TSubclassOf<class UCameraShake> CameraShakeClass;

public:
	UD_DeathBlowPrimary();
	virtual void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);

	
};
