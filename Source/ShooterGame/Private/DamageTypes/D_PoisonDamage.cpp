// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_PoisonDamage.h"


UD_PoisonDamage::UD_PoisonDamage() {


	BaseDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	LowDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	MedDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	HighDamageColor = FLinearColor::FLinearColor(230, 0, 255);
}

void UD_PoisonDamage::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior) {
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


