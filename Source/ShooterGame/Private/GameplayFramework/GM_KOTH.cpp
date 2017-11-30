// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "GM_KOTH.h"

AGM_KOTH::AGM_KOTH() {

}

void AGM_KOTH::DefaultTimer() {
	if (MyGameState && MyGameState->RemainingTime > 0 && !MyGameState->bTimerPaused) {
		MyGameState->RemainingTime--;

		if ((MyGameState->Team1Statistics[(int32)ETeamStatistics::RE_SCORE] <= 0 || MyGameState->Team2Statistics[(int32)ETeamStatistics::RE_SCORE] <= 0) && !bProperMatch) {
			UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - RE_PROPERMATCH"));
			bProperMatch = true;
			FinishingMatch();
		}

		if (MyGameState->RemainingTime <= 0) {
			if (GetMatchState() == MatchState::WaitingPostMatch && !bWaitingPostMatch) {
				UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - WaitingPostMatch"));
				bWaitingPostMatch = true;
				DestroySession();
				SyncronizingAllPlayerStateCharacters();
			}
			else if (GetMatchState() == MatchState::InProgress) {
				if (MyGameState->InProgressState == EInProgressStateEnum::RE_EARLYSTAGE && !bEarlyStage) {
					UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - RE_EARLYSTAGE"));
					bEarlyStage = true;
					HandleMatchProperMatch();
				}
				//else if (MyGameState->InProgressState == EInProgressStateEnum::RE_PROPERMATCH && !bProperMatch) {
				//	UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - RE_PROPERMATCH"));
				//	bProperMatch = true;
				//	FinishingMatch();
				//}
			}
			else if (GetMatchState() == MatchState::WaitingToStart && !bWaitingToStart) {
				bWaitingToStart = true;
				UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - bWaitingToStart"));
				StartMatch();
			}
		}
	}
}


int32 AGM_KOTH::GetWinnerTeam() {
	if (MyGameState->Team1Statistics[(int32)ETeamStatistics::RE_SCORE] > MyGameState->Team2Statistics[(int32)ETeamStatistics::RE_SCORE]) {
		return 0;
	}
	else if (MyGameState->Team1Statistics[(int32)ETeamStatistics::RE_SCORE] < MyGameState->Team2Statistics[(int32)ETeamStatistics::RE_SCORE]) {
		return 1;
	}
	return -1;
}


void AGM_KOTH::HandleMatchProperMatch_Implementation(){
	Super::HandleMatchProperMatch();
}

void AGM_KOTH::HandleMatchEarlyStage_Implementation(){
	Super::HandleMatchEarlyStage();
}