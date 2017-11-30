// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Massive.generated.h"

/**
 * 
 */


UCLASS()
class SHOOTERGAME_API UM_Massive : public UMedal
{
	GENERATED_BODY()


		UM_Massive();


	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
