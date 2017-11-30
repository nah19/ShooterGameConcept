// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_RangerTexas.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_RangerTexas : public UMedal
{
	GENERATED_BODY()

		UM_RangerTexas();
	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
