// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "A_DeathBlow.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_DeathBlow : public AAbility
{
	GENERATED_BODY()
	
	
public:

	AA_DeathBlow();

	UFUNCTION()
    virtual	bool IsAbleToFire() override;

	virtual	void UseAbility() override;

	UFUNCTION(BlueprintNativeEvent, Category = "AbilityProtected")
		void Attack();
	
	//virtual bool UseAbility() override;
	
};
