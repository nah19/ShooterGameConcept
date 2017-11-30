// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptPlayerStart.h"


AConceptPlayerStart::AConceptPlayerStart( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer ) {
	//bReplicates = true;
	//bReplicateMovement = true;
	//SetReplicates( true );
	//GetCapsuleComponent()->SetMobility(EComponentMobility::Movable);

	//StateOfLastPlayerSpawned = NULL;
}

bool AConceptPlayerStart::CheckIfBusy( ) {
	if (StateOfLastPlayerSpawned == NULL) {
		return false;
	}
	return true;
}

void AConceptPlayerStart::RemovePlayerStateLink( ) {
	if (StateOfLastPlayerSpawned != NULL) {
		StateOfLastPlayerSpawned->LastSpawnPoint = NULL;
		StateOfLastPlayerSpawned = NULL;
	}
}


void AConceptPlayerStart::SetPlayerStateLink( AConceptPlayerState* PlayerStateParam ) {
	if (PlayerStateParam != NULL) {
		StateOfLastPlayerSpawned = PlayerStateParam;
		PlayerStateParam->LastSpawnPoint = this;
	}
}