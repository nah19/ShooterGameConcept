// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Friendzone.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Friendzone : public UMedal
{
	GENERATED_BODY()

		UM_Friendzone();
	
	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
