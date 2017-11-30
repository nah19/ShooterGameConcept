// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameFramework/Character.h"
#include "Projectile.h"
#include "ConceptCharacter.h"
#include "MinionController.h"
#include "ConceptDamageBehavior.h"
#include "AI/MinionCharacter.h"
#include "Dygger.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API ADygger : public AMinionCharacter
{
	GENERATED_BODY()
	
public:
	int generation;

	ADygger(const FObjectInitializer& ObjectInitializer);
	void NewGeneration();
    
    UFUNCTION(BlueprintCallable, Category="Dygger")
    void OnLand();


protected:
	
	
};
