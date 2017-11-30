// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "PC_MainMenu.h"

APC_MainMenu::APC_MainMenu(){

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = true;
	bEnableTouchOverEvents = true;

}

void APC_MainMenu::SetupInputComponent() {
	Super::SetupInputComponent();
	
}
