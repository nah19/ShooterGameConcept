// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API ALobbyPlayerState : public ASteamPlayerState
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerState();
	
	UPROPERTY(replicated, editanywhere, blueprintreadwrite, category = "LobbyPlayerState")
		int32 Team;
};
