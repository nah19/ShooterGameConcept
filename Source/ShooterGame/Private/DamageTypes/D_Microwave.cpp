// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_Microwave.h"


UD_Microwave::UD_Microwave()
{

	BaseDamageColor = FLinearColor::White;
	LowDamageColor = FLinearColor::White;
	MedDamageColor = FLinearColor::White;
	HighDamageColor = FLinearColor::White;
}

void UD_Microwave::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior)
{
	DamageBehavior->ApplyLifeDamage(Damage, this->GetClass());
	DamageBehavior->ApplyDisruptDamage(DisruptTime);
}