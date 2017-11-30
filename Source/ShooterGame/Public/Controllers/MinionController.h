

#pragma once

#include "AIController.h"
#include "DetourCrowdAIController.h"

#include "MinionController.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AMinionController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMinionController(const FObjectInitializer& ObjectInitializer);

	//AMinionController(const FPostConstructInitializeProperties& PCIP);

	void KillingMinion();


	
};
