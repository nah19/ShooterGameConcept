// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameplayFramework/ConceptPlayerState.h"
#include "PS_KOTH.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API APS_KOTH : public AConceptPlayerState
{
	GENERATED_BODY()

		APS_KOTH();
	
public:
	UFUNCTION()
		virtual void StatsAddTeamwork();


};
