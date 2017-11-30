// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_Vendetta.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_Vendetta : public UMedal
{
	GENERATED_BODY()


		UM_Vendetta();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
