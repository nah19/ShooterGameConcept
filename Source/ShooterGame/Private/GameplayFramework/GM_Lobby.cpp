// NO COPIAR

#include "ShooterGame.h"
#include "GM_Lobby.h"


AGM_Lobby::AGM_Lobby(){


}

TSubclassOf<AGameSession> AGM_Lobby::GetGameSessionClass() const
{
	return AConceptGameSession::StaticClass();
}


AGameSession* AGM_Lobby::GetGameSession(){
	return GameSession;
}