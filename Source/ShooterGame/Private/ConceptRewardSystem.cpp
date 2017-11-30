// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptRewardSystem.h"

float        UConceptRewardSystem::PercentageToGet = 0.1;

UConceptRewardSystem::UConceptRewardSystem(){
	
}

void UConceptRewardSystem::ManageResourcesOnKill(AConceptPlayerState* MyPlayerState, AConceptPlayerState* VictimPlayerState){
	MyPlayerState->Resources[EResourcesEnum::RE_EXP] += 100 + PercentageToGet * VictimPlayerState->Resources[EResourcesEnum::RE_EXP];
}
