// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "A_DeathBlow.h"



AA_DeathBlow::AA_DeathBlow() {
	//TODO: cadencia y municion
}

bool AA_DeathBlow::IsAbleToFire() {
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	return 	!MyCharacter->GetMovementComponent()->IsFalling();;

	return true;
}

void AA_DeathBlow::UseAbility() {
	Super::UseAbility();
	Attack();
}

void AA_DeathBlow::Attack_Implementation(){
	TimeRemainingToFire = Cadency;
	BaseTimeToFire = Cadency;
}