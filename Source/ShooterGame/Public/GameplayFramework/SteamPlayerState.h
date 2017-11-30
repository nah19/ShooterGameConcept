// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameFramework/PlayerState.h"
#include "Steam/SteamStatsInfo.h"
#include "SteamPlayerState.generated.h"

class AConceptStats;
class UConceptUserCloudStats;

UENUM(BlueprintType)
enum class ESteamStats : uint8
{
	RE_Game_Version			UMETA(DisplayName = "Game_Version"),
	RE_Experience			UMETA(DisplayName = "Experience"),
	RE_Level			UMETA(DisplayName = "Level"),
	RE_ELO			UMETA(DisplayName = "ELO"),
	RE_Kills 		UMETA(DisplayName = "Kills"),
	RE_Deaths			UMETA(DisplayName = "Deaths"),
	RE_Assists			UMETA(DisplayName = "Assists"),
	RE_Dashes			UMETA(DisplayName = "Dashes"),
	RE_Jumps			UMETA(DisplayName = "Jumps"),
	RE_Best_Kill_Strike			UMETA(DisplayName = "Best_Kill_Strike"),
	RE_Num_Matches			UMETA(DisplayName = "Num_Matches"),
	RE_Num_Matches_W			UMETA(DisplayName = "Num_Matches_W"),
	RE_Num_Matches_L			UMETA(DisplayName = "Num_Matches_L"),
	RE_Rolls			UMETA(DisplayName = "Rolls"),
	RE_Teleports			UMETA(DisplayName = "Teleports"),
	RE_Damage_Received			UMETA(DisplayName = "Damage_Received"),
	RE_Damage_Done			UMETA(DisplayName = "Damage_Done"),
	RE_Hills_Conquered			UMETA(DisplayName = "Hills_Conquered"),
	RE_Time_Gameplay			UMETA(DisplayName = "Time_Gameplay"),
	RE_Time_Sprinting		UMETA(DisplayName = "Time_Sprinting"),
	RE_Time_Flying			UMETA(DisplayName = "Time_Flying"),
	RE_Teamwork			UMETA(DisplayName = "Teamwork")
};


/**
 * 
 */
UCLASS()
class SHOOTERGAME_API ASteamPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ASteamPlayerState();


	virtual void BeginPlay() override;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "SteamPlayerState")
		APlayerController* SteamPlayerController;
	//UPROPERTY(visibleanywhere, blueprintreadonly, Category = "SteamPlayerState")
	//	USteamStatsInfo* SteamStats;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "SteamPlayerState")
		UConceptUserCloudStats* UserCloudStats;
	//UFUNCTION(BlueprintCallable, Category = "SteamPlayerState")
	//	FName SteamStatEnumToFName(ESteamStats Name);
	//UFUNCTION(BlueprintCallable, Category = "SteamPlayerState")
	//	ESteamStats FNameToSteamStatEnum(FName Name);
	//UFUNCTION(BlueprintCallable, Category = "SteamPlayerState")
	//	float GetStatValue(ESteamStats StatName);
	//UFUNCTION(BlueprintCallable, Category = "SteamPlayerState")
	//	FString GetStatInfo(ESteamStats Name);
	//UFUNCTION(Server, Reliable, WithValidation)
	//	void InitSteamStats_Server();
	//UFUNCTION(NetMulticast, Reliable, WithValidation)
	//	void InitSteamStats_Multicast(); 
	//UFUNCTION()
	//	void InitSteamStats();
	UFUNCTION()
		void InitUserCloudStats();
	//UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "SteamPlayerState")
	//	void WriteStat(ESteamStats StatName, float Value);
	//UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "SteamPlayerState")
	//	void WriteLocalStat(ESteamStats StatName, float Value);
	//UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "SteamPlayerState")
	//	void WriteIncrementLocalStat(ESteamStats StatName, float Value);
	//UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "SteamPlayerState")
	//	void WriteAllStats();
	//UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "SteamPlayerState")
	//	void WriteLeaderboardStat(ESteamStats StatName, float Value);
	//UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "SteamPlayerState")
	//	void WriteIncrementStat(ESteamStats StatName, float Value);
	//UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "SteamPlayerState")
	//	void WriteIncrementLeaderboardStat(ESteamStats StatName);

	virtual void Tick(float DeltaSeconds) override;

};
