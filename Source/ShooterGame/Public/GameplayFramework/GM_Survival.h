// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameplayFramework/ConceptGameMode.h"
#include "AI/MinionCharacter.h"
#include "ConceptGameSession.h"
#include "GM_Survival.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FHordeRound
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		TArray<int32> EnemiesWeight;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		TArray<TSubclassOf<APawn>> EnemiesClasses;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		int32 RoundTime;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		float DifficultyPoints;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
		float FinalTime;


};

UCLASS()
class SHOOTERGAME_API AGM_Survival : public AConceptGameMode
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(visibleinstanceonly, blueprintreadonly, Category = "Gamemode")
		TArray<APawn*> Enemies;

	UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "Gamemode")
		TSubclassOf<AActor> PickUpClass;

	UFUNCTION(BlueprintCallable, Category = "GameSession")
		AGameSession* GetGameSession();

	UFUNCTION()
		void ContinueSpawnEnemies();

	//AGM_Survival();


protected:
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
	virtual void HandleMatchHasStarted() override;

	void DefaultTimer() override;	

	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "Event EarlyStage "))
	void HandleMatchEarlyStage() override;
	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "WaitingHorde "))
	void HandleWaitingHorde();
	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "FightingHorde "))
	void HandleFightingHorde();
	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "ResultsHorde "))
	void HandleResultsHorde();
	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "Show Fase Horde Results"))
	void ShowFaseHordeResults();
	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "Show Light Horde Results"))
	void ShowLightHordeResults();
	UFUNCTION(BlueprintNativeEvent, Category = "Game", meta = (DisplayName = "Show Final Horde Results"))
	void ShowFinalResults();
	UFUNCTION()
	void FillArrayOfEnemies();
	UFUNCTION()
	void SpawnEnemies();


	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
	int32 WaitingHordeTime;
	UPROPERTY(visibleinstanceonly, blueprintreadonly, Category = "Gamemode")
	int32 HordeTime;
	UPROPERTY(editanywhere, blueprintreadonly, Category = "Gamemode")
	int32 ResultsTime;
	UPROPERTY(visibleinstanceonly, blueprintreadonly, Category = "Gamemode")
	int32 MaxEnemies;
	UPROPERTY(visibleinstanceonly, blueprintreadonly, Category = "Gamemode")
	int32 CurrentHordeIndex = 0;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gamemode")
	TArray<FHordeRound> Hordes;
	UPROPERTY(visibleinstanceonly, blueprintreadonly, Category = "Gamemode")
	TArray<TSubclassOf<APawn>> EnemiesClasses;

};
