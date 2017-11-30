// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Abilities/A_BaseWeapon.h"
#include "A_AttractionArea.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_AttractionArea : public AA_BaseWeapon
{
	GENERATED_BODY()
	
    virtual void StartAbility() override;
	
};
