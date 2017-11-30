// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "MinionCharacter.h"
#include "A_Robots.h"


AA_Robots::AA_Robots() {

}

void AA_Robots::UseAbility(){
	Super::UseAbility();

	PlaySound();
	AMinionCharacter* SpawnedMinion = GetWorld()->SpawnActor<AMinionCharacter>(Minion, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
	SpawnedMinion->TeamNumber = 0;
	SpawnedMinion->Target_C = Targetpoint;
	
	UFunction* function = SpawnedMinion->FindFunction("MinionSpawned");
	SpawnedMinion->ProcessEvent(function, this);
	
}

