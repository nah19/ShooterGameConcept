// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "SteamPlayerState.h"
#include "PS_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class APS_MainMenu : public ASteamPlayerState
{
	GENERATED_BODY()

		APS_MainMenu();
public:
	
	virtual void BeginPlay() override;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		FName LocalSessionName;
		
};

