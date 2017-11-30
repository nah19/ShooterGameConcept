// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Masochistic.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Masochistic : public UMedal
{
	GENERATED_BODY()

		UM_Masochistic();
	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
	
};
