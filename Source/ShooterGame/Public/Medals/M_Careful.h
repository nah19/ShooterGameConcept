// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Careful.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Careful : public UMedal
{
	GENERATED_BODY()

		UM_Careful();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
