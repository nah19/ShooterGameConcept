// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medal.h"
#include "M_Killer.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Killer : public UMedal
{
	GENERATED_BODY()

		UM_Killer();
	
	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
