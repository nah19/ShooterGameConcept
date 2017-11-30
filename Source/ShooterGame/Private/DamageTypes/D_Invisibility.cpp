// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_Invisibility.h"

UD_Invisibility::UD_Invisibility(){


	BaseDamageColor = FLinearColor::White;
	LowDamageColor = FLinearColor::White;
	MedDamageColor = FLinearColor::White;
	HighDamageColor = FLinearColor::White;
}

void UD_Invisibility::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior){
	AConceptPlayerController* KilledController = Cast<AConceptPlayerController>(KilledPlayer);
	if (KilledController) {
		KilledController->InitInGameStealthHUD();
		AConceptCharacter* InvisibleChar = Cast<AConceptCharacter>(KilledPlayer->GetPawn());
		if (InvisibleChar) {
			InvisibleChar->MakeInvisible(NewMat, false);
		}
	}
}
