// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageTypes/ConceptDamageType.h"
#include "D_Invisibility.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UD_Invisibility : public UConceptDamageType
{
	GENERATED_BODY()
	
public:
	UD_Invisibility();
	void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);

protected:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageTypes")
		UMaterialInterface* NewMat;
};
