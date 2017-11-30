// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Spree.generated.h"

/**
 * 
 */


UCLASS()
class SHOOTERGAME_API UM_Spree : public UMedal
{
	GENERATED_BODY()


		UM_Spree();


	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
