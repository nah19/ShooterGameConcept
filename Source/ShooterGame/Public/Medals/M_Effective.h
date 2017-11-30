// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Effective.generated.h"

/**
 * 
 */


UCLASS()
class SHOOTERGAME_API UM_Effective : public UMedal
{
	GENERATED_BODY()


		UM_Effective();


	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
