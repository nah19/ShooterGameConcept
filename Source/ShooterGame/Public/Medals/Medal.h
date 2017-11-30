// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
//#include "ConceptStats.h"
#include "Medal.generated.h"

/**
 * 
 */

class AConceptStats;

UCLASS(Blueprintable)
class SHOOTERGAME_API UMedal : public UObject
{
	GENERATED_BODY()
	
	
public:
	UMedal();
	


	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		FText Description;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		float Multiplier;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		UTexture2D* Image;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		int32 TypeIndex;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		FText MedalName;

	UFUNCTION()
	virtual UMedal* GetAchievedMedal(AConceptStats* Stats);
};
