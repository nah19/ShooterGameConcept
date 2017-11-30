// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "LobbyPlayerState.h"




ALobbyPlayerState::ALobbyPlayerState()
{
	


}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, Team);

}