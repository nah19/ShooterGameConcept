// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameplayFramework/ConceptPlayerState.h"
#include "AI/MinionCharacter.h"
#include "YBlinnPawn.h"
#include "GameplayFramework/GM_Survival.h"
#include "PS_Survival.generated.h"

/**
 * 
 */
UCLASS()
class APS_Survival : public AConceptPlayerState
{

	GENERATED_BODY()

		APS_Survival();
	
	void SyncronizingPlayerStateInProgress(EInProgressStateEnum MatchState) override;

public:
	UPROPERTY(visibleinstanceonly, blueprintreadonly, category = "Playerstate")
	int32 ExperiencePoints = 0;
	UPROPERTY(visibleinstanceonly, blueprintreadonly, category = "Playerstate")
	TMap<TSubclassOf<APawn>, int32> TotalEnemiesKilledClasses;
	UPROPERTY(visibleinstanceonly, blueprintreadonly, category = "Playerstate")
	TMap<TSubclassOf<APawn>, int32> RoundEnemiesKilledClasses;
	UPROPERTY(replicated, editanywhere, blueprintreadwrite, Category = "Gamemode")
	TArray<FHordeRound> HordesCompleted;
	UFUNCTION()
	void AddHorde(FHordeRound Horde);
	UFUNCTION(blueprintcallable, category = "Playerstate")
	TArray<int32> ExperienceCalculator();

	void BeginPlay() override;
};
