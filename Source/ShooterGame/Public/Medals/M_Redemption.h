// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Redemption.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Redemption : public UMedal
{
	GENERATED_BODY()

		UM_Redemption();
	
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
