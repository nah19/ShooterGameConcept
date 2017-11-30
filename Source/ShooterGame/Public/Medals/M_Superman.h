// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Superman.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Superman : public UMedal
{
	GENERATED_BODY()
		UM_Superman();
	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
	
	
};
