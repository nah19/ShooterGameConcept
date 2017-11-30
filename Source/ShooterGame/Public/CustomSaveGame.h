// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "CustomSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UCustomSaveGame : public USaveGame
{
	GENERATED_BODY()
	
		UCustomSaveGame(const FObjectInitializer& ObjectInitializer);

public:

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ApplyControlSettings(APlayerController* PlayerController, float MouseSensitivity, float XSensitivity, float YSensitivity, float XSensitivityGamepad, float YSensitivityGamepad, bool XInvert, bool YInvert);
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    FString GetResolutionCommand(int32 XResolution, int32 YResolution, FString Mode);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Change Localization"), Category = "Locale")
	static void ChangeLocalization(FString target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load Localization"), Category = "Locale")
	static void LoadLocalization();

	UPROPERTY(Blueprintreadwrite, Visibleanywhere, category = "settings")
	bool bGamepadController;

};
