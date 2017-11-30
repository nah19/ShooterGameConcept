// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "DamageTypes/ConceptDamageType.h"
#include "D_SlowDown.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UD_SlowDown : public UConceptDamageType
{
	GENERATED_BODY()
    
public:
    UD_SlowDown();
    void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);
	
    void DamageEffect(float Duration, float Damage, UConceptDamageBehavior* DamageBehavior);
	
};
