// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameplayFramework/ConceptPlayerState.h"
#include "PS_TeamDeathmatch.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API APS_TeamDeathmatch : public AConceptPlayerState
{
	GENERATED_BODY()

		APS_TeamDeathmatch();

public:
	UFUNCTION()
		virtual void StatsAddTeamwork();
};