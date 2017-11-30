// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_FriendlyDeath.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_FriendlyDeath : public UMedal
{
	GENERATED_BODY()

		UM_FriendlyDeath();

		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
