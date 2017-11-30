// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "ConceptCharacter.h"
#include "ConceptDamageType.generated.h"

UCLASS()
class SHOOTERGAME_API UConceptDamageType : public UDamageType
{
	GENERATED_BODY()

	public:
		UConceptDamageType();
		
	void HandleDamage(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior, AConceptGameMode* MyGameMode, FAbilityData AbilityData);

	virtual void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);
    virtual void DamageEffect(float Duration, float Damage,UConceptDamageBehavior* DamageBehavior);

	UPROPERTY(BlueprintReadOnly, Category = "DamageType")
		FLinearColor BaseDamageColor;
	UPROPERTY(BlueprintReadOnly, Category = "DamageType")
		FLinearColor LowDamageColor;
	UPROPERTY(BlueprintReadOnly, Category = "DamageType")
		FLinearColor MedDamageColor;
	UPROPERTY(BlueprintReadOnly, Category = "DamageType")
		FLinearColor HighDamageColor;
	UPROPERTY(BlueprintReadOnly, Category = "DamageType")
		bool bAllowFriendlyFire = false;

};
