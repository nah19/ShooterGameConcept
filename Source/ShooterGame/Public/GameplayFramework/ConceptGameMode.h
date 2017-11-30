// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "ConceptPlayerStart.h"
#include "ConceptDamageType.h"
#include "ConceptPlayerController.h"
#include "ConceptGameMode.generated.h"



UCLASS(minimalapi)
class AConceptGameMode : public AGameMode {
	GENERATED_UCLASS_BODY()


protected:
	UPROPERTY()
		AConceptGameState* MyGameState;
	UPROPERTY()
		int32 CurrentPawnType;
	UPROPERTY()
		int32 NumberOfPlayersT1;
	UPROPERTY()
		int32 NumberOfPlayersT2;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "Gamemode")
		int32 CurrentPlayers;

	UPROPERTY()
		bool bWaitingPostMatch = false;
	UPROPERTY()
		bool bEarlyStage = false;
	UPROPERTY()
		bool bProperMatch = false;
	UPROPERTY()
		bool bWaitingToStart = false;


	UFUNCTION()
		void ReactiveAllCharacters();
	UFUNCTION()
		void DisableAllCharacters();
	UFUNCTION()
		virtual void HandleMatchEarlyStage();
	UFUNCTION()
		virtual void HandleMatchProperMatch();
	UFUNCTION()
		virtual bool IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const;
	UFUNCTION()
		virtual bool IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Player) const;
	UFUNCTION()
		void LiberateAllSpawnPoints();
	UFUNCTION()
		void SortPlayerStartsByPriority(AController* Player, TArray<APlayerStart*> &PreferredSpawns, TArray<APlayerStart*> &FallbackSpawns);
	UFUNCTION()
		void DeadEvent(FKillData Data);
	UFUNCTION()
		void FillAllPlayerStateWithGameInstanceData();
	UFUNCTION()
		void FillMedals();
	UFUNCTION(BlueprintCallable, category ="session")
		void  SyncronizingAllPlayerStateCharacters();
	UFUNCTION()
		void FinishingMatch();




	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
	virtual void PostInitializeComponents() override;
	virtual void PreLogin(const FString& Options, const FString& Address, const TSharedPtr<const FUniqueNetId>& UniqueId, FString& ErrorMessage) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void DefaultTimer();
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void PreInitializeComponents() override;


public:
	
	

	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		int32 TotalGamePlayers;


	UFUNCTION(BlueprintCallable, Category = "Gamemode")
	virtual void Killed(AController* Killer, AController* KilledPlayer, FAbilityData AbilityData);
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
		void LiberateSpawnPoint(AConceptPlayerStart* SpawnPoint);
	UFUNCTION()
		virtual int32 GetWinnerTeam(); 
	UFUNCTION()
		void InactivePlayer(AConceptPlayerController* Controller);
	UFUNCTION()
		int32 TeamSelector();
	UFUNCTION(BlueprintCallable, Category = "Game")
		void UpdatePlayerControllers();
	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "CustomPostLogin"))
		void CustomPostLogin(APlayerController* NewPlayer);
	UFUNCTION(BlueprintCallable, Category = "GameSession")
		AGameSession* GetGameSession();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
		void DestroySession();
	UFUNCTION()
		void DelayedLogin(AConceptPlayerController* NewPlayer);


	private:
		UFUNCTION(exec)
			void FinishMatch();


};



