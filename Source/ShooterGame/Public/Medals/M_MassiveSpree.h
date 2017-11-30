// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medal.h"
#include "M_MassiveSpree.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_MassiveSpree : public UMedal
{
	GENERATED_BODY()


		UM_MassiveSpree();
	UMedal* GetAchievedMedal(AConceptStats* Stats) override;
};
