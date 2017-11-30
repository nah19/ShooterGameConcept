// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_AfterDeath.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_AfterDeath : public UMedal
{
	GENERATED_BODY()
		UM_AfterDeath();
	
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;

	
};
