// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Flash.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Flash : public UMedal
{
	GENERATED_BODY()


		UM_Flash();

	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
