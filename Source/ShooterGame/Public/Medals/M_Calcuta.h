// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Calcuta.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Calcuta : public UMedal
{
	GENERATED_BODY()
		UM_Calcuta();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
	
};
