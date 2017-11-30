// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ConceptDamageType.h"
#include "D_AssaultWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UD_AssaultWeapon : public UConceptDamageType
{
	GENERATED_BODY()
	
	class UClass* CamShake;
	TSubclassOf<class UCameraShake> CameraShakeClass;

public:
	UD_AssaultWeapon();
	virtual void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);
	
	
};
