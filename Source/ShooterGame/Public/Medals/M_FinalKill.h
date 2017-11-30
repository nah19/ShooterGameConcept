// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_FinalKill.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_FinalKill : public UMedal
{
	GENERATED_BODY()

		UM_FinalKill();

		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
