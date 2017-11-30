// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_LetMeOne.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_LetMeOne : public UMedal
{
	GENERATED_BODY()

		UM_LetMeOne();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
