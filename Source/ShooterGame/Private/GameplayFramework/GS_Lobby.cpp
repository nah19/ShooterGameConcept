// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "NetGameInstance.h"
#include "GS_Lobby.h"



AGS_Lobby::AGS_Lobby() {

	LobbyTime = 1;
	bWaitingToStart = false;
	this->SetActorTickEnabled(true);
	MapName = "PruebaNueva";
	bTraveling = false;
}

void AGS_Lobby::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (bWaitingToStart) {
		LobbyTime -= DeltaSeconds;
		if (LobbyTime <= 0 && !bTraveling) {
			OnEveryoneReady();
		}
	} else {
		LobbyTime += DeltaSeconds;
	}
}

void AGS_Lobby::OnEveryoneReady() {
	CleanAllWidgets();
	//if (HasAuthority()) {
	//	UNetGameInstance* NetGameInstance = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	//	//TODO: Selector de mapa/modo
	//	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, TEXT("ServerTravelTo"));

	//	if (SurvivalOnlyMapName == "Survival") {
	//		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, TEXT("ServerTravelTo --- "+ SurvivalOnlyMapName));
	//		NetGameInstance->ServerTravelTo("/Game/Maps/" + SurvivalOnlyMapName);
	//	}
	//	else {
	//		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, TEXT("ServerTravelTo --- "+ MapName));
	//		NetGameInstance->ServerTravelTo("/Game/Maps/" + MapName);
	//	}
	//	
	//}
	UNetGameInstance* NetGameInstance = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	if (HasAuthority()) {
		bTraveling = true;
		NetGameInstance->ServerTravelTo("/Game/Maps/" + MapName);
	}
}

void AGS_Lobby::SyncronizeClientsTimer_Implementation(float LobbyTimer, float ServerOffsetTime) {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("-----  GS timer: %f"),LobbyTimer));
	float LagTime = GetServerWorldTimeSeconds() - ServerOffsetTime;
	LobbyTime = LobbyTimer - LagTime;
}

void AGS_Lobby::CleanAllWidgets_Implementation() {
	for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr) {
		UUserWidget* LiveWidget = *Itr;
		if (!LiveWidget->GetWorld()) {
			continue;
		} else {
			if (!LiveWidget->GetName().Contains("Arrow") && !LiveWidget->GetName().Contains("Crosshair") && !LiveWidget->GetName().Contains("Fade")) {
				LiveWidget->RemoveFromParent();
			}
		}
	}
}

void AGS_Lobby::GetOnlySurvivalMode_Implementation() {
	FString LocalMapName;
	UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
	if (GI && GI->SessionSettings.IsValid()) {
		GI->SessionSettings->Get(SETTING_GAMEMODE, LocalMapName);
		if (LocalMapName.Contains("Survival")) {
			bIsSurvivalOnly = true;
		}
		else {
			bIsSurvivalOnly = false;
		}
	}
}


void AGS_Lobby::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGS_Lobby, bWaitingToStart);
	DOREPLIFETIME(AGS_Lobby, bIsSurvivalOnly);
}

bool AGS_Lobby::GetOnlySurvivalMode_Validate() { return true; }