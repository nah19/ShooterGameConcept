// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "ConceptGameState.h"
#include "ConceptPlayerController.h"
#include "NetGameInstance.h"
#include "Steam/SteamStatsInfo.h"
#include "PlayerSaveGame.h"
#include "SteamPlayerState.h"
#include "ConceptPlayerState.generated.h"


/**
 * 
 */
class AConceptGameMode;
class AConceptPlayerStart;
class AAbility;
class AConceptStats;

UENUM( BlueprintType )
enum class EStateEnum : uint8 {
	RE_INITIAL_SPECTATOR 	UMETA(DisplayName = "RE_INITIAL_SPECTATOR"),
	RE_END_SPECTATOR 		UMETA(DisplayName = "RE_END_SPECTATOR"),
	RE_BLOCKED 				UMETA(DisplayName = "RE_BLOCKED" ),
	RE_PLAYING 				UMETA(DisplayName = "RE_PLAYING" ),
	RE_KILLED 				UMETA(DisplayName = "RE_KILLED" ),
	RE_RESULTS				UMETA(DisplayName = "RE_RESULTS"),
	RE_RESPAWNING			UMETA(DisplayName = "RE_RESPAWNING"),
	RE_LEAVER				UMETA(DisplayName = "RE_LEAVER"),
};



UENUM( BlueprintType )
namespace ERaceEnum {
	enum Race {
		RE_UFG 			UMETA( DisplayName = "UFG" ),
		RE_EVO			UMETA( DisplayName = "EVO" ),
		RE_CONTINUUM	UMETA( DisplayName = "CONTINUUM" ),
		RE_NHC			UMETA( DisplayName = "NHC" ),
	};
}

UENUM( BlueprintType )
namespace EResourcesEnum {
	enum Resources {
		RE_ALTENUM 	UMETA( DisplayName = "ALTENUM" ),
		RE_COINS 	UMETA( DisplayName = "COINS" ),
		RE_EXP		UMETA( DisplayName = "EXP" ),
	};
}


UCLASS( )
class SHOOTERGAME_API AConceptPlayerState : public ASteamPlayerState {
	
public:
	GENERATED_BODY( )
	AConceptPlayerState();

	FTimerHandle RespawnHandle;
	FTimerHandle DeathHandle;

	TSubclassOf<class AAbility> Ab1;

	TSubclassOf<class AAbility> Ab2;

	TSubclassOf<class AAbility> Ab3;

	TSubclassOf<class AAbility>	Ab4;

	TSubclassOf<class AAbility>	Ab5;

	int32 PreviousKills;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		FName LocalSessionName;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		AConceptStats* StatsDataBase;
	//UPROPERTY()
	//	TSubclassOf<class AConceptStats> StatsDataBaseClass;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
			UClass* ClassToSpawn;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
			int32 Team = -1;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
			TArray<FPlayerPersistentData> AllPlayersData;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		class AConceptCharacter *MyCharacter;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		class AConceptGameMode *MyGameMode;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		class AConceptGameState *MyGameState;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		class AConceptPlayerController *MyPlayerController;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float TimeToReappear;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float TimeDeath;
	UPROPERTY( replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState" )
		int32 Lvl;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		int32 Experience;
	UPROPERTY(replicated, editanywhere, blueprintreadwrite, Category = "PlayerState" )
		int32 Kills;
	UPROPERTY(replicated, editanywhere, blueprintreadwrite, Category = "PlayerState" )
		int32 Deaths;
	UPROPERTY( replicated, editanywhere, blueprintreadwrite, Category = "PlayerState" )
		int32 Assist;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		UConceptUserCloudStats* PlayerDefaults;
	UPROPERTY()
		AConceptPlayerStart* LastSpawnPoint;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		TEnumAsByte<ERaceEnum::Race> Race;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		EStateEnum State;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		TArray<float> Resources;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
		TArray<AAbility*> CharacterAbilities;
	UFUNCTION(Client, Reliable, WithValidation)
		void UpToSteamMatchesResults(int32 Winner);
	UFUNCTION( Server, Reliable, WithValidation )
		void SyncronizingPlayerState( AConceptGameMode* MyGameModeParam, FName MatchState );
	UFUNCTION(Client, Reliable, WithValidation)
		void SyncronizeStatsWithSteam();
	UFUNCTION()
		void InitState();
	UFUNCTION()
		void InitOwnState();
	UFUNCTION()
		void InitStrangerState();
	UFUNCTION()
		void InitGameAttributes();
	UFUNCTION()
		virtual void SyncronizingPlayerStateInProgress( EInProgressStateEnum MatchState );
	UFUNCTION(Client, Reliable, WithValidation)
		void SetDataOnGameInstanceFromSaved();
	UFUNCTION(Server, Reliable, WithValidation)
		void SetPlayerDataGameInstance(UClass* DefaultCharacter, UClass* DefaultAbility1, UClass* DefaultAbility2, UClass* DefaultAbility3);
	UFUNCTION(Server, Reliable, WithValidation)
		void DeathCharacter();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerInitAbilities(TSubclassOf<AAbility> Class1, TSubclassOf<AAbility> Class2, TSubclassOf<AAbility> Class3, TSubclassOf<AAbility> Class4, TSubclassOf<AAbility> Class5);
	UFUNCTION(Server, Reliable, WithValidation)
		void RemoveAbilities();
	UFUNCTION( NetMulticast, Reliable, WithValidation )
		void SetPlayerState( EStateEnum NextState );
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void SetTimeDeathMulticast(float Time);
	UFUNCTION()
		void OnJoinStateManager();
	UFUNCTION( Server, Reliable, WithValidation )
		void RestartPlayerWithState( EStateEnum StateParam );
	UFUNCTION()
		void ScoreKill( );
	UFUNCTION()
		void ScoreDeath(  );
	UFUNCTION(Server, Reliable, WithValidation)
		void ScoreAssist();
	UFUNCTION(Server, Reliable, WithValidation, blueprintcallable, Category="PlayerState")
		void OnDeathStateManager();
	UFUNCTION()
		void OnRespawnStateManager();
	UFUNCTION()
		void OnDeathSpectatorStateManager();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeToSpectator();
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientChangeToSpectator(AConceptPlayerStart* Start);
	UFUNCTION()
		void KillEventToHUD(FString KillerGameTag,  FString VictimGameTag, FString KillerNameAbility, int32 KillerTeam, int32 VictimTeam);
	UFUNCTION(NetMulticast, WithValidation, UnReliable, Category = "PlayerState")
		void KillEventToHUD_Multicast(const FString& KillerGameTag, const FString& VictimGameTag, const FString& KillerNameAbility, int32 KillerTeam, int32 VictimTeam);
	UFUNCTION(Server, reliable, withvalidation,Category = "PlayerState")
		void FillVars();
	UFUNCTION(netmulticast, reliable, blueprintcallable, Category = "PlayerState")
		void FillVariablesWithGameInstanceData(UClass* MyClass, int32 MyTeam, const TArray<FPlayerPersistentData>& PlayersData);
	UFUNCTION(Server, reliable, withvalidation, Category = "PlayerState")
		void DelayedFillVars(UClass* MyClass, int32 MyTeam, FPlayerPersistentData PlayerData);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyCharacter(AConceptCharacter* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyPlayerController(AConceptPlayerController* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyGameMode(AConceptGameMode* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyGameState(AConceptGameState* Char);

	UFUNCTION(Client, Reliable)
		void SetSavedGameDefaults(int32 TeamSelector);
	UFUNCTION(Client, Reliable, WithValidation)
		void SetServerSavedGameDefaults(int32 TeamSelector);
	UFUNCTION()
		void DestroyPlayerState();




	UFUNCTION()
		void InitStatsDataBase();
	UFUNCTION(blueprintcallable, Category = "PlayerState", Server, Reliable, WithValidation)
		void InitStatsDataBase_Server();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void InitStatsDataBase_Multicast();
	UFUNCTION(Server, Reliable, WithValidation)
		void SetELOReplicated_Server(int32 ELOParam);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void SetELOReplicated_Multicast(int32 ELOParam);


	UFUNCTION(Server, reliable, withvalidation)
		void StatsKillScored();
	UFUNCTION(Server, reliable, withvalidation)
		void StatsDeathScored();
	UFUNCTION(Server, reliable, withvalidation)
		void StatsAssistScored();
	UFUNCTION(Server, reliable, withvalidation)
		void StatsMostAssist(const TArray<AConceptPlayerState*>& PlayerStates);
	UFUNCTION(Server, reliable, withvalidation)
		void StatsHightestRatio(const TArray<AConceptPlayerState*>& PlayerStates);
	UFUNCTION(Server, reliable, withvalidation)
		void StatsFinalKill(AConceptPlayerState* KillerPlayerState);
	UFUNCTION(Server, withvalidation, reliable)
		void StatsLessDamagePlayer_Server(const TArray<AConceptPlayerState*>& PlayerStates);
	UFUNCTION(NetMulticast, withvalidation, reliable)
		void StatsLessDamagePlayer_Multicast(const TArray<AConceptPlayerState*>& PlayerStates, const TArray<float>& Damages);


	UFUNCTION()
		void StatsAddDash();
	UFUNCTION()
		void StatsAddJump();
	UFUNCTION()
		void StatsAddDamageReceived(float DamageParam);
	UFUNCTION()
		void StatsAddDamageDone(float DamageParam);
	UFUNCTION(Server, withvalidation, reliable)
		void StatsReplicateDamages_Server();
	UFUNCTION(NetMulticast, withvalidation, reliable)
		void StatsReplicateDamages_Multicast(float DamageParam, float DamageDoneParam);
	UFUNCTION()
		void StatsAddGravityChanges();
	UFUNCTION(blueprintcallable, Category = "PlayerState")
		void StatsAddHillConquered();
	UFUNCTION()
		void StatsAddNumMatches();
	UFUNCTION()
		void StatsAddNumMatchesW();
	UFUNCTION()
		void StatsAddNumMatchesL();
	UFUNCTION()
		void StatsAddRolls();
	UFUNCTION()
		void StatsAddTeleport();
	UFUNCTION()
		void StatsAddPlayersTeamBalance();
	UFUNCTION()
		void StatsAddTimeGameplay(float DeltaTime);
	UFUNCTION()
		void StatsAddTimeSprinting(float DeltaTime);
	UFUNCTION()
		void StatsAddTimeFlying(float DeltaTime);
	UFUNCTION()
		void StatsAddTimeWalking(float DeltaTime);
	UFUNCTION(Client, Reliable, WithValidation)
		void StatsFriendlyFireDone();
	UFUNCTION()
		void StatsOnKillPlayerStateComparation(AConceptPlayerState* VictimPlayerState);
	UFUNCTION()
		void StatsOnDeathPlayerStateComparation(AConceptPlayerState* KillerPlayerState);
	UFUNCTION()
		void StatsFirstKill();
	UFUNCTION()
		void StatsFirstDeath();
	UFUNCTION()
		virtual void StatsAddTeamwork();
	UFUNCTION()
		virtual void StatsAddLeaguePoints();


	virtual void CopyProperties(class APlayerState* PlayerState) override;

	protected:

		UFUNCTION(Server, Reliable, WithValidation)
			void AttachAbilities(AAbility* AbilityToPass, AConceptCharacter* MyCharacterParam);
		UFUNCTION()
			TArray<AAbility*> InitAbilities(TSubclassOf<AAbility> Class1, TSubclassOf<AAbility> Class2, TSubclassOf<AAbility> Class3, TSubclassOf<AAbility> Class4, TSubclassOf<AAbility> Class5);
		UFUNCTION()
			void ChangeToSpectator(AConceptPlayerStart* Start);
		UFUNCTION(Client, Reliable)
			void OnJoinProperMatch_Client();

		virtual void BeginPlay() override;
		virtual void PostInitializeComponents() override;
		virtual void Tick(float DeltaSeconds) override;
};