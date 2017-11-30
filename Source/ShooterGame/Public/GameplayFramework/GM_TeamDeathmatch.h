// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameplayFramework/ConceptGameMode.h"
#include "GM_TeamDeathmatch.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTERGAME_API AGM_TeamDeathmatch : public AConceptGameMode
{
	GENERATED_BODY()

public:
		AGM_TeamDeathmatch();
		UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "Event ProperMatch "))
			void HandleMatchProperMatch() override;
		virtual int32 GetWinnerTeam() override;

};
