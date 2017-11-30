// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_NoFear.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_NoFear : public UMedal
{
	GENERATED_BODY()

		UM_NoFear();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
