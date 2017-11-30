// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "A_Robots.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_Robots : public AAbility
{
	GENERATED_BODY()

public:

	AA_Robots();
	TSubclassOf<class AMinionCharacter> Minion;
	virtual void UseAbility() override;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Minion")
		AActor* Targetpoint;
	

};
