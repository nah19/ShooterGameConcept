// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ConceptPlayerState.h"
#include "ConceptRewardSystem.generated.h"

UCLASS()
class UConceptRewardSystem : public UObject
{
	GENERATED_BODY()

public:
	UConceptRewardSystem();
	static float PercentageToGet;

	static void ManageResourcesOnKill(AConceptPlayerState* MyPlayerState, AConceptPlayerState* VictimPlayerState);
	static void ManageResourcesOnMinionKill(AConceptPlayerState* MyPlayerState, TArray<float> GainedResources);
};