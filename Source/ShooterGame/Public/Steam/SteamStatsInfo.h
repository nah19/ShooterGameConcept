// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SteamStat.h"
#include "NetGameInstance.h"
#include "ConceptPlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineStats.h"
#include "OnlineAchievementsInterface.h"
#include "OnlineLeaderboardInterface.h"
#include "OnlineAchievementsInterface.h"
//#include "OnlineSubsystemSteamPackage.h"
#include "SteamStatsInfo.generated.h"

/**
*
*/


USTRUCT()
struct FStat
{
	GENERATED_BODY()

		UPROPERTY()
		float StatValue;

	FString StatName;

	//Constructor
	FStat()
	{
		StatValue = 0;
		StatName = "EMPTY";
	}
};

//USTRUCT()
//struct FLocalOnlineStat
//{
//	GENERATED_BODY()
//	/** The id of the achievement */
//	FString Id;
//
//	/** The progress towards completing this achievement: 0.0-100.0 */
//	double Progress;
//
//	/** Returns debugging string to print out achievement info */
//	FString ToDebugString() const
//	{
//		return FString::Printf(TEXT("Id='%s', Progress=%f"),
//			*Id,
//			Progress
//		);
//	}
//
//	void FillWithOnlineStat(FOnlineStat OnlineStat)
//	{
//		Id = OnlineStat.Id;
//		Progress = OnlineStat.Progress;
//	}
//};

//USTRUCT(BlueprintType)
//struct FLeaderboardRow
//{
//	GENERATED_BODY()
//
//	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
//	float StatValue;
//	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
//	int32 Rank;
//	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
//	FString UserName;
//	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
//	FString PlayerID;
//
//	//Constructor
//	FLeaderboardRow()
//	{
//		StatValue = 0;
//		UserName = "EMPTY";
//		Rank = -1;
//	}
//};

UCLASS()
class SHOOTERGAME_API USteamStatsInfo : public UObject
{
	GENERATED_BODY()

public:

	USteamStatsInfo();

	//TArray<FOnlineAchievement> OutAchievements;
	//TArray<FOnlineStat> OutStats;
	//TArray<TSharedRef<FOnlineUser>> UsersData;
	//
	//UPROPERTY()
	//	ULocalPlayer* LocalPlayer;
	//UPROPERTY(Replicated)
	//	TArray<FLocalOnlineStat> LocalStats;
	////UPROPERTY(Replicated)
	////	TArray<FLocalOnlineStat> LocalLeaderboardStats;
	////UPROPERTY(VisibleAnywhere, Blueprintreadonly, category = "Steamstats")
	////	TArray<FLeaderboardRow> LeaderboardReadeableData;
	////UPROPERTY(VisibleAnywhere, Blueprintreadonly, category = "Steamstats")
	////	TArray<UTexture2D*> LeaderboardReadeableTextures;
	//UPROPERTY(Replicated)
	//	FName SessionName;
	//UPROPERTY()
	//	AActor* MyPlayerController;

	//
	////void GetCachedSteamStats(const bool bWasSuccessful, TArray<FString> Nombres);	
	////void OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	////void OnQueryStatsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	////void OnQueryStatsCompleteForFill(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	////void OnWriteStatsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	////TArray<TSharedRef<FOnlineUser>> GetAllUserData();
	//FLocalOnlineStat GetStat(FName Name);
	////void OnCompleteLeaderboard(FOnlineLeaderboardReadRef Leaderboard);
	//void WriteLocalStat(FName StatName, float Value);
	//

	////UFUNCTION(server, reliable, withvalidation, BlueprintCallable, Category = "Online")
	////void QueryAchievements(int32 Value = 0);
	//UFUNCTION(server, reliable, withvalidation, BlueprintCallable, Category = "Online")
	//	void InitStats(ULocalPlayer* LPlayer, FName SessionNameParam);
	////UFUNCTION(server, reliable, withvalidation, BlueprintCallable, Category = "Online")
	////	void WriteStat(FName StatName, float Value);
	////UFUNCTION(server, reliable, withvalidation, BlueprintCallable, Category = "Online")
	////	void WriteLeaderboardStat(FName SessionNameParam, FName StatName, float Value);
	////UFUNCTION(server, reliable, withvalidation, BlueprintCallable, Category = "Online")
	////	void GetAllUserValuesOfLeaderboard(FName NameOfLeaderboard);
	////UFUNCTION(server, reliable, withvalidation, BlueprintCallable, Category = "Online")
	////	void ResetStats(bool ResetAchieventsToo);
	//UFUNCTION(BlueprintCallable, Category = "Online")
	//	void UpdateStat(FName StatName);
	//UFUNCTION(server, reliable, withvalidation, BlueprintCallable, Category = "Online")
	//	void FillStatsWithSteamStats();
	//UFUNCTION(BlueprintCallable, Category = "Online")
	//	void UpdateAllStatsToSteam();
	//UFUNCTION(BlueprintCallable, Category = "Online")
	//	void IncrementLocalStat(FName Name, float Increment);
	//UFUNCTION(BlueprintCallable, Category = "Online")
	//	float GetStatValue(FName Name);
	//UFUNCTION(BlueprintCallable, Category = "Online")
	//	FString GetStatInfo(FName Name);
	////UFUNCTION(BlueprintCallable, Category = "Online")
	////	int32 GetNumberOfUsersConected();

	//

};