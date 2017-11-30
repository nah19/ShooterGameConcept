// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "A_TurretShoot.h"
#include "A_Turrets.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_Turrets : public AAbility
{
	GENERATED_BODY()
	
public:

	AA_Turrets();
	TSubclassOf<class AA_TurretShoot> Turret;
	virtual void UseAbility() override;
	TArray < AA_TurretShoot* > SpawnedTurrets;
	int32 OrientationY;
	
};
