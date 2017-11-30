// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "A_Invisibility.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_Invisibility : public AAbility
{
	GENERATED_BODY()
	
public:

	AA_Invisibility();

	virtual void UseAbility() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material)
	UMaterialInterface* NewMat;


private:
	AConceptCharacter* InvisibleCharacter;

	UFUNCTION(Server, Reliable, withvalidation)
		void SwitchVisibility(int32 Visible);


};