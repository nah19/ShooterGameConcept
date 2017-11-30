
// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "PS_MainMenu.h"


APS_MainMenu::APS_MainMenu() {



}

void APS_MainMenu::BeginPlay() {
	Super::BeginPlay();

//	InitSteamStats_Server();
	LocalSessionName = SessionName;
}
