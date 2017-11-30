// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Medals/Medal.h"
#include "M_BestFriend.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UM_BestFriend : public UMedal
{
	GENERATED_BODY()

		UM_BestFriend();
		UMedal* GetAchievedMedal(AConceptStats* Stats) override;
	
};
