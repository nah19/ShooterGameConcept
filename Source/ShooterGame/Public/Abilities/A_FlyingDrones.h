// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Abilities/Ability.h"
#include "A_FlyingDrones.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_FlyingDrones : public AAbility
{
	GENERATED_BODY()

public:
    AA_FlyingDrones();
	
    virtual void UseAbility();
	
};
