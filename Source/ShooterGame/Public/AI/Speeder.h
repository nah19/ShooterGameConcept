// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Projectile.h"
#include "ConceptCharacter.h"
#include "MinionController.h"
#include "ConceptDamageBehavior.h"
#include "MinionCharacter.h"
#include "Speeder.generated.h"


UCLASS()
class SHOOTERGAME_API ASpeeder : public AMinionCharacter
{
	GENERATED_BODY()

public:
	int generation;

	ASpeeder(const FObjectInitializer& ObjectInitializer);
	void NewGeneration();

protected:


};
