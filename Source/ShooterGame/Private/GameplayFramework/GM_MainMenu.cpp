// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GM_MainMenu.h"
#include "PC_MainMenu.h"


AGM_MainMenu::AGM_MainMenu ( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer ) {
	//PlayerControllerClass = APC_MainMenu::StaticClass( );
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MainMenuSpectator"));

	//if (PlayerPawnBPClass.Class != NULL) {
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

}

//FVector2D AGM_MainMenu::GetNativeResolution()
//{
//	FVector2D ToReturn;
//
//	FDisplayMetrics Metrics;
//	FDisplayMetrics::GetDisplayMetrics(Metrics);
//	ToReturn = FVector2D(Metrics.PrimaryDisplayWidth, Metrics.PrimaryDisplayHeight);
//
//	return ToReturn;
//}

AGameSession* AGM_MainMenu::GetGameSession() { return GameSession; }