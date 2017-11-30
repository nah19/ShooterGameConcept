// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "A_SatelliteAttack.h"


AA_SatelliteAttack::AA_SatelliteAttack() {
	//TODO: cadencia y municion
	
}

void AA_SatelliteAttack::UseAbility(){
	Super::UseAbility();

	OnRaySpawned();



}

