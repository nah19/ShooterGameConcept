// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "DamageTypes/ConceptDamageType.h"
#include "D_Snare.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UD_Snare : public UConceptDamageType
{
	GENERATED_BODY()
	
public:
	UD_Snare();
	void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);


	
	
};
