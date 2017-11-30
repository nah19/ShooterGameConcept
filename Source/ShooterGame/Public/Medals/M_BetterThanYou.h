// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_BetterThanYou.generated.h"

/**
 * 
 */


UCLASS()
class SHOOTERGAME_API UM_BetterThanYou : public UMedal
{
	GENERATED_BODY()


		UM_BetterThanYou();


	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
