// NO COPIAR

#pragma once

#include "GameFramework/GameMode.h"
#include "ConceptGameSession.h"
#include "GM_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGM_Lobby : public AGameMode
{
	GENERATED_BODY()
	
public:
		UFUNCTION(BlueprintCallable, Category = "GameSession")
		AGameSession* GetGameSession();

		AGM_Lobby();


protected: 
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
	
};
