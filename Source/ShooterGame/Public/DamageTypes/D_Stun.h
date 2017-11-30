// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "DamageTypes/ConceptDamageType.h"
#include "D_Stun.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UD_Stun : public UConceptDamageType
{
	GENERATED_BODY()
	
	
public:
	UD_Stun();
	void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);

    void DamageEffect(float Duration, float Damage, UConceptDamageBehavior* DamageBehavior);
};
