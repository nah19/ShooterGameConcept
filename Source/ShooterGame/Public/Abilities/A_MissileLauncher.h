// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "A_BaseWeapon.h"
#include "A_MissileLauncher.generated.h"



UCLASS()
class SHOOTERGAME_API AA_MissileLauncher : public AA_BaseWeapon
{
	GENERATED_BODY()

public:

	AA_MissileLauncher();

	virtual void UseAbility() override;

	virtual void StartAbility() override;
	
};
