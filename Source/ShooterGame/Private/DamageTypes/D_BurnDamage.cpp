// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_BurnDamage.h"



UD_BurnDamage::UD_BurnDamage() {


	BaseDamageColor = FLinearColor::Red;
	LowDamageColor = FLinearColor::Red;
	MedDamageColor = FLinearColor::Red;
	HighDamageColor = FLinearColor::Red;
}

void UD_BurnDamage::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior) {
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
}


