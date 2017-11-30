// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptGameMode.h"
#include "ConceptDamageType.h"


UConceptDamageType::UConceptDamageType() {
	BaseDamageColor = FLinearColor::Red;
	LowDamageColor = FLinearColor::Red;
	MedDamageColor = FLinearColor::Red;
	HighDamageColor = FLinearColor::Red;
}


void UConceptDamageType::HandleDamage(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior, AConceptGameMode* MyGameMode, FAbilityData AbilityData) {
	if (DamageBehavior && DamageBehavior->IsPawnDied()) { return; }

	DamagePlayer(Damage, KillerPlayer, KilledPlayer, DamageBehavior);
    if(AbilityData.EffectDuration > 0.f) {
        DamageEffect(AbilityData.EffectDuration, Damage, DamageBehavior);
    }

	if (DamageBehavior->IsPawnDied()){
		DamageBehavior->Killed(MyGameMode, KillerPlayer, KilledPlayer, AbilityData);
	}
}

void UConceptDamageType::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior){
}

void UConceptDamageType::DamageEffect(float Duration, float Damage, UConceptDamageBehavior* DamageBehavior){
}




