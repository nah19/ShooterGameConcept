// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Granma.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Granma : public UMedal
{
	GENERATED_BODY()

		UM_Granma();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
	
};
