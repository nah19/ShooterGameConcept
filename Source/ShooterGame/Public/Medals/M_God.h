// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medal.h"
#include "M_God.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_God : public UMedal
{
	GENERATED_BODY()


		UM_God();
	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
