// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_FriendlyFire.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_FriendlyFire : public UMedal
{
	GENERATED_BODY()
		UM_FriendlyFire();
	
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
