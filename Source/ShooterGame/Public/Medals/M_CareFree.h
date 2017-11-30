// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_CareFree.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_CareFree : public UMedal
{
	GENERATED_BODY()

		UM_CareFree();
	
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
