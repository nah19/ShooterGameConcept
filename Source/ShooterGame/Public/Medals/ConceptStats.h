// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Actor.h"
#include "Medal.h"
#include "ConceptPlayerState.h"
#include "ConceptStats.generated.h"

/**
 * 
 */
//class AConceptPlayerState;


UCLASS()
class SHOOTERGAME_API AConceptStats : public AActor
{
	GENERATED_BODY()
	
public:

	
	AConceptStats();
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState"	)
		AConceptPlayerState* ParentPlayerState;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float Dashes;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float Jumps;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float GravityChanges;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float HillsConquered;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float NumMatches;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float NumMatchesW;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float NumMatchesL;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float Rolls;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float Teamwork;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float Teleports;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
		float TimeGameplay;

	UPROPERTY(editanywhere, blueprintreadwrite, Category = "PlayerState")
	int32 PlayersTeamBalance;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "PlayerState")
	int32 KillStrike;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	int32 ELO;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
	int32 InitialELO;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
	int32 Assist;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
	int32 Kills;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "PlayerState")
	int32 Deaths;
	UPROPERTY()
	bool HightestRatio;
	UPROPERTY()
	bool MostAssists;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool FirstKill;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool FirstDeath;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool FinalKill;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool FinalDeath;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	TArray<AConceptPlayerState*> Killers;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	TArray<AConceptPlayerState*> Victims;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float Damage;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float DamageDone;
	UPROPERTY()
	bool LessDamage;
	UPROPERTY()
	bool MVP;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float Experience;
	UPROPERTY()
	bool Death;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool FriendlyFire;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool FriendlyDeath;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool Nemesis;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool NemesisDying;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool NemesisVendeta;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	bool AfterDeath;
	UPROPERTY()
	AConceptPlayerState* MyNemesis;
	UPROPERTY()
	int32 KillLastEnemyWhoKilledYou;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float TimeSprinting;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float TimeFlying;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float TimeCrouched;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float TimeWalking;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	int32 KillYourself;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	TArray<UMedal*> Medals;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float ExperienceMultiplier = 0;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	int32 PlayersTeam0;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	int32 PlayersTeam1;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "PlayerState")
	float TeamBalance;

	UPROPERTY()
	TArray<UMedal*> ActiveMedals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		UConceptUserCloudStats* UserCloudStats;

	UFUNCTION()
	void InitStats(AConceptPlayerState* MyPlayerState);
	UFUNCTION()
	void DelayedInitStats();
	UFUNCTION()
	void SyncronizeStatsWithSteam();
	UFUNCTION()
		void SyncronizeIncrementableStats();
	UFUNCTION()
		void SyncronizeOverridableStats();
	UFUNCTION(Client, reliable, withvalidation)
	void CheckMedals();
	UFUNCTION()
		void AddTeamWorkInTeamDeathMatch();
	UFUNCTION()
		void AddTeamWorkInKOTH();
	UFUNCTION()
		void AddLeaguePoints();
	UFUNCTION()
	void AddMedal(UMedal * Medal);
	UFUNCTION()
	void OnKillScored();
	UFUNCTION()
	void OnDeathScored();
	UFUNCTION()
	void OnAssistScored();
	UFUNCTION()
	void AddTimeSprinting(float DeltaTime);
	UFUNCTION()
	void AddTimeFlying(float DeltaTime);
	UFUNCTION()
	void AddTimeWalking(float DeltaTime);
	UFUNCTION()
	void FriendlyFireDone();

	UFUNCTION()
		void AddDash();
	UFUNCTION()
		void AddJump();
	UFUNCTION()
		void AddGravityChanges();
	UFUNCTION()
		void AddHillConquered();
	UFUNCTION()
		void AddNumMatches();
	UFUNCTION()
		void AddNumMatchesW();
	UFUNCTION()
		void AddNumMatchesL();
	UFUNCTION()
		void AddRolls();
	UFUNCTION()
		void AddTeamwork(float DeltaTime);
	UFUNCTION()
		void AddTeleport();
	UFUNCTION()
		void AddTimeGameplay(float DeltaTime);
	UFUNCTION()
		void AddPlayersByTeam(int32 Team0, int32 Team1);

	UFUNCTION()
		void AddDamageReceived(float DamageReceivedParam);
	UFUNCTION()
		void AddDamageDone(float DamageDoneParam);
	UFUNCTION(Server, Reliable, WithValidation)
	void CheckMostAssists(const TArray<AConceptPlayerState*>& PlayerStates);
	UFUNCTION(Server, Reliable, WithValidation)
	void CheckHightestRatio(const TArray<AConceptPlayerState*>& PlayerStates);
	UFUNCTION()
	void OnKillPlayerStateComparation(AConceptPlayerState* VictimPlayerState);
	UFUNCTION()
	void OnDeathPlayerStateComparation(AConceptPlayerState* KillerPlayerState);
	UFUNCTION()
		void SetFirstKill();
	UFUNCTION()
		void SetFirstDeath();
	UFUNCTION()
		void SetFinalKill(AConceptPlayerState* KillerPlayerState);

};
