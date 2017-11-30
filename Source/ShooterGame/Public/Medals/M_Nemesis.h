// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Nemesis.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Nemesis : public UMedal
{
	GENERATED_BODY()

		UM_Nemesis();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
