// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "GM_TeamDeathmatch.h"


AGM_TeamDeathmatch::AGM_TeamDeathmatch()
{
	
}


int32 AGM_TeamDeathmatch::GetWinnerTeam() {
	if (MyGameState->Team1Statistics[(int32)ETeamStatistics::RE_KILLS] > MyGameState->Team2Statistics[(int32)ETeamStatistics::RE_KILLS]) {
		return 0;
	}
	else if (MyGameState->Team1Statistics[(int32)ETeamStatistics::RE_KILLS] < MyGameState->Team2Statistics[(int32)ETeamStatistics::RE_KILLS]) {
		return 1;
	}
	return -1;
}


void AGM_TeamDeathmatch::HandleMatchProperMatch_Implementation() {
	Super::HandleMatchProperMatch();
}



