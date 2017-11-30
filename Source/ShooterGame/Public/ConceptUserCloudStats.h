// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameFramework/SaveGame.h"
#include "SteamPlayerState.h"
#include "ConceptUserCloudStats.generated.h"



/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UConceptUserCloudStats : public USaveGame
{
	GENERATED_BODY()
	
public:
	UConceptUserCloudStats();
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Kills;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Deaths;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Assist;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Dashes;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 KillStrike;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 NumMatches;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 NumMatchesW;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 NumMatchesL;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Experience;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		TMap<int32, int32> MedalsIndexQuantity;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Rolls;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Teleports;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 DamageReceived;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 HillsConquered;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		float TimeGameplay;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		float TimeSprinting;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		float TimeFlying;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Teamwork;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 DamageDone;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 Jumps;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		int32 ELO;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		TArray<int32> MedalsIndex;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UserCloudStats")
		TArray<int32> MedalsQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCloudStats")
		TArray<TSubclassOf<class UObject>> DefaultAbilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCloudStats")
		TSubclassOf<class UObject> DefaultCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCloudStats")
		int32 GAMEVERSION;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCloudStats")
		int32 Level;


	UPROPERTY()
		AConceptStats* ConceptStats;
	UFUNCTION()
		void Init(AConceptStats* ConceptStatsParam);
	UFUNCTION()
		void GetFromConceptStats(AConceptStats* ConceptStatsParam);
	UFUNCTION()
		void PushToConceptStats(AConceptStats* ConceptStatsParam);
	UFUNCTION(Blueprintcallable, Category = "UserCloudStats")
		void ToString();
	UFUNCTION(Blueprintcallable, Category = "UserCloudStats")
		float GetValueOfStat(ESteamStats Name);
	UFUNCTION(Blueprintcallable, Category = "UserCloudStats")
		FString GetNameOfStat(ESteamStats Name);

};
