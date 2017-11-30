// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_GiveLifePoints.h"

UD_GiveLifePoints::UD_GiveLifePoints() {

	bAllowFriendlyFire = true;
	BaseDamageColor = FLinearColor::Green;
	LowDamageColor = FLinearColor::Green;
	MedDamageColor = FLinearColor::Green;
	HighDamageColor = FLinearColor::Green;
}

void UD_GiveLifePoints::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior) {
	AConceptPlayerController* KilledController = Cast<AConceptPlayerController>(KilledPlayer);
	if (KilledController) {
		KilledController->InitInGameHealthHUD();
	}

	DamageBehavior->ApplyLifeRecovery(Damage);

	//Cast<AConceptPlayerController>(KillerPlayer)->GetConceptCharacter()->MyDamageBehavior->ApplyLifeDamage(Damage);
}


