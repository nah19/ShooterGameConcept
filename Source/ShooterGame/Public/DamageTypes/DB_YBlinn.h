// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "DamageTypes/ConceptDamageBehavior.h"
#include "YBlinnPawn.h"
#include "DB_YBlinn.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UDB_YBlinn : public UConceptDamageBehavior
{
    GENERATED_BODY()
    
public:
    AYBlinnPawn* MyMinion;
    
    UDB_YBlinn();
    
    void Initialize(AActor* ActorToApplybehavior) override;
    
    void ApplyLifeDamage(float Damage, TSubclassOf<UDamageType> DamageType = NULL) override;
    
    bool IsPawnDied() override;
    
    void Killed(AConceptGameMode* MyGameMode, AController* KillerPlayer, AController* KilledPlayer, FAbilityData AbilityData) override;
	
	
	
	
};
