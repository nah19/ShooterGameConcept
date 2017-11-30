// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "GS_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGS_Lobby : public AGameState
{
	GENERATED_BODY()

public:

		AGS_Lobby();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="LobbyState")
		float LobbyTime;

	UPROPERTY(Replicated, EditAnyWhere, BlueprintReadWrite, Category = "LobbyState")
		bool bIsSurvivalOnly;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "LobbyState")
		FString MapName;

	UPROPERTY(replicated, EditAnyWhere, BlueprintReadWrite, Category = "LobbyState")
		bool bWaitingToStart;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "LobbyState")
		void SyncronizeClientsTimer(float LobbyTimer, float ServerOffsetTime);
		virtual void SyncronizeClientsTimer_Implementation(float LobbyTimer, float ServerOffsetTime);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "LobbyState")
		void CleanAllWidgets();
	virtual void CleanAllWidgets_Implementation();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "LobbyState")
		void GetOnlySurvivalMode();
	//UFUNCTION(BlueprintCallable, Category = "LobbyState")
	//	bool GetOnlySurvivalModeMulticast();

private:

	virtual void Tick(float DeltaSeconds) override;
	void OnEveryoneReady();
	bool bTraveling; 
};
