// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_BePositive.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_BePositive : public UMedal
{
	GENERATED_BODY()

		UM_BePositive();

	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
