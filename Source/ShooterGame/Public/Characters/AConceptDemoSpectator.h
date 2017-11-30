// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "AConceptDemoSpectator.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AAConceptDemoSpectator : public ASpectatorPawn
{
	GENERATED_BODY()
	
    AAConceptDemoSpectator();
	
    
    void GoToTimeSeconds(float PlayBackTime);
    
    UFUNCTION(blueprintcallable, category="DemoNet")
    void MoveAlongClip(float ForwardBackwardAmount);
	
};
