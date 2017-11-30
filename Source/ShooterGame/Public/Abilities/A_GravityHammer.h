// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "A_GravityHammer.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_GravityHammer : public AAbility
{
	GENERATED_BODY()
	
public:

	AA_GravityHammer();

	virtual bool IsAbleToFire() override;
	virtual void StartAbility() override;
};
