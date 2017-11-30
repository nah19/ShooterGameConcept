// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Helper.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Helper : public UMedal
{
	GENERATED_BODY()
		UM_Helper();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
	
};
