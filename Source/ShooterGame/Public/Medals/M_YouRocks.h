// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_YouRocks.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_YouRocks : public UMedal
{
	GENERATED_BODY()
		UM_YouRocks();
	
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
