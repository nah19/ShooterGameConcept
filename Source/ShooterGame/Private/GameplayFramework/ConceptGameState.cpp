// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptStats.h"
#include "ConceptGameState.h"



AConceptGameState::AConceptGameState() {

	NumTeams = 2;
	RemainingTime = 0;
	bTimerPaused = false;
	InProgressState = EInProgressStateEnum::RE_EARLYSTAGE; //RE_WAITINGTOBEINPROGRESS;
	MatchState = MatchState::WaitingToStart;
	Team1Statistics.SetNumUninitialized(5);
	Team2Statistics.SetNumUninitialized(5);
	NetUpdateFrequency = 5;
	NetCullDistanceSquared = 125000000.0;

	EarlyStageTime = 5;
	RoundTime = 300;
	WarmupTime = 10;
	TimeBetweenMatches = 10;
	TypeOfGameMode = ETypeOfGameMode::RE_DEATHMATCH;
	GameModeName = "GAMEMODE NAME";

	Team1Statistics[0] = 0;
	Team1Statistics[1] = 0;
	Team1Statistics[2] = 0;
	Team1Statistics[3] = 0;
	Team1Statistics[4] = 1000;

	Team2Statistics[0] = 0;
	Team2Statistics[1] = 0;
	Team2Statistics[2] = 0;
	Team2Statistics[3] = 0;
	Team2Statistics[4] = 1000;
}

void AConceptGameState::InitMedals_Implementation() {
	float MostExperience = 0;
	float Damage = 100000;
	float MostAssist = 0;
	float Deaths = 100000;
	AConceptPlayerState* PlayerWithMostExperience = NULL;
	AConceptPlayerState* PlayerWithLessDamage = NULL;
	AConceptPlayerState* PlayerWithMostAssist = NULL;
	AConceptPlayerState* PlayerWithLessDeaths = NULL;

	TArray<AConceptPlayerState*> PlayerStates;

	for (TActorIterator<AConceptPlayerState> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		AConceptPlayerState* CurrentPlayerState = *ActorItr;
		if (CurrentPlayerState) {
			PlayerStates.Add(CurrentPlayerState);
		}
	}


	for (TActorIterator<AConceptPlayerState> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		AConceptPlayerState* CurrentPlayerState = *ActorItr; 

		CurrentPlayerState->StatsMostAssist(PlayerStates);
		CurrentPlayerState->StatsHightestRatio(PlayerStates);
		CurrentPlayerState->StatsFinalKill(LastKiller);
		//CurrentPlayerState->StatsLessDamagePlayer_Server(PlayerStates);	TO DO

		
		//CurrentPlayerState->StatsLessDamagePlayerChecker_Client(PlayerStates);
		//if (CurrentPlayerState->StatsDataBase) {

		//	CurrentPlayerState->StatsDataBase->Kills = CurrentPlayerState->Kills;
		//	CurrentPlayerState->StatsDataBase->Assist = CurrentPlayerState->Assist;
		//	CurrentPlayerState->StatsDataBase->Deaths = CurrentPlayerState->Deaths;

		//	if (CurrentPlayerState->Experience >= MostExperience) {
		//		MostExperience = CurrentPlayerState->Experience;
		//		PlayerWithMostExperience = CurrentPlayerState;
		//	}





		//	if (CurrentPlayerState->StatsDataBase->Deaths <= Deaths) {
		//		Deaths = CurrentPlayerState->StatsDataBase->Deaths;
		//		PlayerWithLessDeaths = CurrentPlayerState;
		//	}


		//	CurrentPlayerState->Experience = 500 + (CurrentPlayerState->Kills * 100) + (CurrentPlayerState->Assist * 50) - (CurrentPlayerState->Deaths * 10);
		if (CurrentPlayerState->StatsDataBase) {
			CurrentPlayerState->StatsDataBase->CheckMedals();
		}
		//}
	}

	//if (PlayerWithMostExperience && PlayerWithMostExperience->StatsDataBase) {
	//	PlayerWithMostExperience->StatsDataBase->MVP = true;
	//}

	//if (PlayerWithLessDamage && PlayerWithLessDamage->StatsDataBase) {
	//	PlayerWithLessDamage->StatsDataBase->LessDamage = true;
	//}	
}

bool AConceptGameState::SetMedalsOnKill_Validate(AConceptPlayerState* VictimPlayerState, AConceptPlayerState* KillerPlayerState) {
	return true;
}

void AConceptGameState::SetMedalsOnKill_Implementation(AConceptPlayerState* VictimPlayerState, AConceptPlayerState* KillerPlayerState) {

	if (!VictimPlayerState || !KillerPlayerState) { return; }

	KillerPlayerState->StatsOnKillPlayerStateComparation(VictimPlayerState);
	VictimPlayerState->StatsOnDeathPlayerStateComparation(KillerPlayerState);

	if (!FirstKiller) {
		VictimPlayerState->StatsFirstDeath();
		KillerPlayerState->StatsFirstKill();
		FirstKiller = true;
	}

	LastKiller = KillerPlayerState;
}

bool AConceptGameState::InitMedals_Validate() {
	return true;
}

void AConceptGameState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const {
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AConceptGameState, NumTeams );
	DOREPLIFETIME( AConceptGameState, RemainingTime );
	DOREPLIFETIME( AConceptGameState, bTimerPaused );
	DOREPLIFETIME( AConceptGameState, TeamScores );
	DOREPLIFETIME( AConceptGameState, InProgressState );
	DOREPLIFETIME( AConceptGameState, Team1Statistics);
	DOREPLIFETIME( AConceptGameState, Team2Statistics);
	DOREPLIFETIME( AConceptGameState, DataOfAllKills);
	DOREPLIFETIME( AConceptGameState, LastKiller);
	
}


void AConceptGameState::AddKillInfo(FKillData Data){
	DataOfAllKills.Add(Data);
}