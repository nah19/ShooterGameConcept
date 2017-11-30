// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_TakeLifePoints.h"

UD_TakeLifePoints::UD_TakeLifePoints() {


	BaseDamageColor = FLinearColor::Green;
	LowDamageColor = FLinearColor::Green;
	MedDamageColor = FLinearColor::Green;
	HighDamageColor = FLinearColor::Green;
}

void UD_TakeLifePoints::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior) {
	AConceptPlayerController* KilledController = Cast<AConceptPlayerController>(KilledPlayer);
	if (KilledController) {
		//KilledController->InitInGameDamageHUD();
		AConceptCharacter*  KilledCharacter = Cast<AConceptCharacter>(KilledController->GetPawn());
		if (KilledCharacter) {
			KilledCharacter->ShakeManager(CameraShakeClass, false, Damage / 10);
		}
	}

	AConceptPlayerController* KillerController = Cast<AConceptPlayerController>(KillerPlayer);

	DamageBehavior->ApplyLifeDamage(Damage, this->GetClass());

	Cast<AConceptPlayerController>(KillerPlayer)->GetConceptCharacter()->MyDamageBehavior->ApplyLifeRecovery(Damage);
}


