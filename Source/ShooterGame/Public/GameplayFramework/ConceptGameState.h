// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "StaticFunctionLibrary.h"
#include "ConceptGameState.generated.h"

UENUM(BlueprintType)
enum class ETypeOfGameMode : uint8 {
	RE_DEATHMATCH 	UMETA(DisplayName = "RE_DEATHMATCH"),
	RE_KOTH 	UMETA(DisplayName = "RE_KOTH"),
	RE_SURVIVAL 	UMETA(DisplayName = "RE_SURVIVAL ")
};


UENUM( BlueprintType )
enum class EInProgressStateEnum : uint8 {
	RE_WAITINGTOBEINPROGRESS 	UMETA( DisplayName = "WAITINGTOBEINPROGRESS" ),
	RE_EARLYSTAGE 	UMETA( DisplayName = "EARLYSTAGE" ),
	RE_PROPERMATCH 	UMETA( DisplayName = "PROPERMATCH" ),
	RE_WAITING_HORDE 	UMETA( DisplayName = "RE_WAITING_HORDE"),
	RE_FIGHTING_HORDE 	UMETA( DisplayName = "RE_FIGHTING_HORDE"),
	RE_RESULTS_HORDE 	UMETA( DisplayName = "RE_RESULTS_HORDE"),
};

UENUM(BlueprintType)
enum class ETeamStatistics : uint8 {
	RE_KILLS	UMETA(DisplayName = "RE_KILLS"),
	RE_DEATHS	UMETA(DisplayName = "RE_DEATHS"),
	RE_MINIONS	UMETA(DisplayName = "RE_MINIONS"),
	RE_TURRETS	UMETA(DisplayName = "RE_TURRETS"),
	RE_SCORE	UMETA(DisplayName = "RE_SCORE"),
};
/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AConceptGameState : public AGameState
{
	GENERATED_BODY()
	
	
	public:
		AConceptGameState();


	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		int32 EarlyStageTime;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		int32 RoundTime;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		int32 TimeBetweenMatches;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		int32 WarmupTime;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		ETypeOfGameMode TypeOfGameMode;
	UPROPERTY()
		FString GameModeName;



	UPROPERTY()
		bool FirstKiller;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
		TArray<FKillData> DataOfAllKills;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
		AConceptPlayerState* LastKiller;
	UPROPERTY( Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode" )
		EInProgressStateEnum InProgressState;
	UPROPERTY( Transient, Replicated )
		int32 NumTeams;
	UPROPERTY( Transient, Replicated )
		TArray<int32> TeamScores;
	UPROPERTY( Transient, Replicated, EditInstanceOnly, blueprintreadwrite, Category = "Gamemode" )
		int32 RemainingTime;
	UPROPERTY( Transient, Replicated )
		bool bTimerPaused;
	

	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "GameMode")
		TArray<int32> Team1Statistics;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "GameMode")
		TArray<int32> Team2Statistics;

	UFUNCTION()
		void AddKillInfo(FKillData Data);
	UFUNCTION(Client, withvalidation, reliable)
		void InitMedals();
	UFUNCTION(server, withvalidation, reliable)
		void SetMedalsOnKill(AConceptPlayerState* VictimPlayerState, AConceptPlayerState* KillerPlayerState);
};
