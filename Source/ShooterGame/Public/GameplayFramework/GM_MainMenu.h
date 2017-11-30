// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GM_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class AGM_MainMenu : public AGameMode
{
	GENERATED_BODY()
		AGM_MainMenu(const FObjectInitializer& ObjectInitializer);

public:
	//UFUNCTION(BlueprintCallable, Category = "Gamemode")
	//FVector2D GetNativeResolution();

	UFUNCTION(BlueprintCallable, Category = "Gamemode")
	AGameSession* GetGameSession();
}; 