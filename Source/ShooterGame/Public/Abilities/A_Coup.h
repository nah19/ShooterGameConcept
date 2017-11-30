// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "A_Coup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_Coup : public AAbility
{
	GENERATED_BODY()
	
public:

	AA_Coup();
	virtual void UseAbility() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* ForceParticle;

	UPROPERTY()
	URadialForceComponent* forcePush;
	
};
