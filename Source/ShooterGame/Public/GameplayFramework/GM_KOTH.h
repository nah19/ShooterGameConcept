// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameplayFramework/ConceptGameMode.h"
#include "GM_KOTH.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGM_KOTH : public AConceptGameMode
{
	GENERATED_BODY()

public:


	AGM_KOTH();

	virtual int32 GetWinnerTeam() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "Event ProperMatch "))
		void HandleMatchProperMatch() override;
	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "Event EarlyStage "))
		void HandleMatchEarlyStage() override;
	
	virtual void DefaultTimer() override;

};
