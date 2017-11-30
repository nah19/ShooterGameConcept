// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "OnlineSubsystem.h"
#include "OnlineAchievementsInterface.h"
#include "OnlineIdentityInterface.h"
#include "OnlineStats.h"
//#include "OnlineSubsystemSteam.h"
#include "StaticFunctionLibrary.h"
#include "Steam/SteamStatsInfo.h"


USteamStatsInfo::USteamStatsInfo()
{
	//UsersData.SetNumUninitialized(0);
	//LocalStats.SetNumUninitialized(0);
}

//void USteamStatsInfo::QueryAchievements_Implementation(int32 Value)
//{
//	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
//	{
//		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
//
//		if (SubsystemInstance)
//		{
//			/** Make sure we got the player **/
//			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
//			if (Identity.IsValid())
//			{
//				/** Make sure the player controller actually exists **/
//				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
//				if (UserId.IsValid())
//				{
//					IOnlineAchievementsPtr AchievementsInterface = SubsystemInstance->GetAchievementsInterface();
//					if (AchievementsInterface.IsValid())
//					{
//						switch (Value) {
//						case 0: {
//							AchievementsInterface->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &USteamStatsInfo::OnQueryAchievementsComplete));
//						}
//								break;
//						case 1: {
//							AchievementsInterface->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &USteamStatsInfo::OnQueryStatsComplete));
//						}
//								break;
//						case 2: {
//							AchievementsInterface->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &USteamStatsInfo::OnQueryStatsCompleteForFill));
//						}
//								break;
//						}
//					}
//				}
//			}
//		}
//	}
//
//}


//void USteamStatsInfo::GetCachedSteamStats(const bool bWasSuccessful, TArray<FString> Nombres)
//{
//	TArray<FOnlineStat> Array;
//	Array.SetNumUninitialized(0);
//
//	if (bWasSuccessful)
//	{
//		if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
//		{
//
//			IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
//
//			if (SubsystemInstance)
//			{
//				/** Make sure we got the player **/
//				IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
//				if (Identity.IsValid())
//				{
//					/** Make sure the player controller actually exists **/
//					TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
//					if (UserId.IsValid())
//					{
//						IOnlineAchievementsPtr AchievementsInterface = SubsystemInstance->GetAchievementsInterface();
//						if (AchievementsInterface.IsValid())
//						{
//							OutStats.SetNumUninitialized(0);
//							AchievementsInterface->GetCachedStats(*UserId.Get(), Nombres, OutStats);
//							GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("GET CACHED STATS")));
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void USteamStatsInfo::WriteStat_Implementation(FName StatName, float Value)
//{
//	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
//	{
//		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
//
//		if (SubsystemInstance)
//		{
//			/** Make sure we got the player **/
//			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
//			if (Identity.IsValid())
//			{
//				/** Make sure the player controller actually exists **/
//				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
//				if (UserId.IsValid())
//				{
//					IOnlineAchievementsPtr AchievementsInterface = SubsystemInstance->GetAchievementsInterface();
//
//					if (AchievementsInterface.IsValid())
//					{
//						AchievementsInterface->WriteStats(*UserId.Get(), FOnStatsWrittenDelegate::CreateUObject(this, &USteamStatsInfo::OnWriteStatsComplete), StatName.ToString(), Value);
//						WriteLocalStat(StatName, Value);
//						//AchievementsInterface->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &USteamStatsInfo::OnQueryAchievementsComplete));
//						//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, FString::Printf(TEXT("WriteStat_Implementation: %s - %f"), *StatName.ToString(), Value));
//					}
//				}
//			}
//		}
//	}
//}
//
//TArray<TSharedRef<FOnlineUser>> USteamStatsInfo::GetAllUserData()
//{
//	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
//	{
//		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
//
//		if (SubsystemInstance)
//		{
//			/** Make sure we got the player **/
//			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
//			if (Identity.IsValid())
//			{
//				/** Make sure the player controller actually exists **/
//				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
//				if (UserId.IsValid())
//				{
//					
//					IOnlineUserPtr UsersInterface = SubsystemInstance->GetUserInterface();
//
//					if (UsersInterface.IsValid())
//					{
//						UsersInterface->GetAllUserInfo(0, UsersData);
//					}
//				}
//			}
//		}
//	}
//
//	return UsersData;
//}
//
//int32 USteamStatsInfo::GetNumberOfUsersConected() {
//	return GetAllUserData().Num();
//}


//void USteamStatsInfo::ResetStats_Implementation(bool ResetAchieventsToo)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ResetStats_Implementation ");
//	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
//	{
//		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
//
//		if (SubsystemInstance)
//		{
//			/** Make sure we got the player **/
//			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
//			if (Identity.IsValid())
//			{
//				/** Make sure the player controller actually exists **/
//				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
//				if (UserId.IsValid())
//				{
//					
//					IOnlineAchievementsPtr AchievementsInterface = SubsystemInstance->GetAchievementsInterface();
//
//					if (AchievementsInterface.IsValid())
//					{
//						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ResetStats_Implementation FINAL");
//						AchievementsInterface->ResetStats(ResetAchieventsToo);
//					}
//				}
//			}
//		}
//	}
//}

//bool USteamStatsInfo::ResetStats_Validate(bool ResetAchieventsToo) {
//	return true;
//}

//void USteamStatsInfo::WriteLeaderboardStat_Implementation(FName SessionName, FName StatName, float Value)
//{
//
//	FOnlineLeaderboardWrite WriteObject;
//	WriteObject.LeaderboardNames.Add(StatName);
//	WriteObject.RatedStat = StatName;
//	WriteObject.DisplayFormat = ELeaderboardFormat::Number;
//	WriteObject.SortMethod = ELeaderboardSort::Descending;
//	WriteObject.UpdateMethod = ELeaderboardUpdateMethod::Force;
//	WriteObject.SetIntStat(StatName, (int32)Value);
//
//	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
//	{
//		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
//
//		if (SubsystemInstance)
//		{
//			/** Make sure we got the player **/
//			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
//			if (Identity.IsValid())
//			{
//				/** Make sure the player controller actually exists **/
//				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
//				if (UserId.IsValid())
//				{
//					IOnlineLeaderboardsPtr LeaderboardsInterface = SubsystemInstance->GetLeaderboardsInterface();
//					if (LeaderboardsInterface.IsValid())
//					{
//						LeaderboardsInterface->WriteLeaderboards(SessionName, *UserId, WriteObject);
//						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("WriteLeaderboardStat_Implementation: %f"), Value));
//						WriteLocalStat(StatName, Value);
//						//LeaderboardsInterface->FlushLeaderboards(SessionName);
//					}
//				}
//			}
//		}
//	}
//}


//void USteamStatsInfo::GetAllUserValuesOfLeaderboard_Implementation(FName NameOfLeaderboard)
//{
//	FOnlineLeaderboardReadRef Leaderboard(new FOnlineLeaderboardRead());
//	Leaderboard->LeaderboardName = NameOfLeaderboard;
//	Leaderboard->SortedColumn = "0";
//	Leaderboard->ReadState = EOnlineAsyncTaskState::NotStarted;
//
//	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
//	{
//		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
//		if (SubsystemInstance)
//		{
//			/** Make sure we got the player **/
//			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
//			if (Identity.IsValid())
//			{
//				if (true)
//				{
//
//					IOnlineLeaderboardsPtr LeaderboardsInterface = SubsystemInstance->GetLeaderboardsInterface();
//					LeaderboardsInterface->LeaderboardCallback.BindUObject(this, &USteamStatsInfo::OnCompleteLeaderboard);
//
//					if (LeaderboardsInterface.IsValid())
//					{
//						LeaderboardsInterface->ReadLeaderboardsByRange(-400, 400, Leaderboard);
//					}
//				}
//			}
//		}
//	}
//}
//
//void USteamStatsInfo::OnCompleteLeaderboard(FOnlineLeaderboardReadRef Leaderboard) {
//	LeaderboardReadeableData.SetNumUninitialized(0);
//	TArray<FName> ColumnsData;
//	for (int32 i = 0; i < Leaderboard->Rows.Num(); i++) {
//		FLeaderboardRow Row;
//		Row.UserName = Leaderboard->Rows[i].NickName;
//		TArray<FVariantData> Keys;
//		Leaderboard->Rows[i].Columns.GenerateValueArray(Keys);
//		int32 Value;
//		Keys[0].GetValue(Value);
//		Row.StatValue = Value;
//		Row.Rank = Leaderboard->Rows[i].Rank;
//		Row.PlayerID = Leaderboard->Rows[i].PlayerId->ToString();
//		LeaderboardReadeableData.Add(Row);
//	}
//}
//
//void USteamStatsInfo::OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
//{
//	if (bWasSuccessful)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "OnQueryAchievementsComplete 0");
//		FillStatsWithSteamStats();
//	}
//	else
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Not successful onquery");
//}

//void USteamStatsInfo::OnQueryStatsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
//{
//	if (bWasSuccessful)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "OnQueryStatsComplete 0");
//		FillStatsWithSteamStats();
//	}
//	else
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Not successful onquery");
//}
//
//void USteamStatsInfo::OnQueryStatsCompleteForFill(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
//{
//	if (bWasSuccessful)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "OnQueryStatsCompleteForFill 0");
//		FillStatsWithSteamStats();
//	}
//	else
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Not successful onquery");
//}
//
//void USteamStatsInfo::OnWriteStatsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
//{
//	if (bWasSuccessful)
//	{
//		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "writeadooo");
//	}
//	else {
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Not successful onquery");
//	}
//}


//void USteamStatsInfo::UpdateStat(FName Stat) {
//	for (int32 i = 0; i < LocalStats.Num(); i++)
//	{
//		if (LocalStats[i].Id.Equals(Stat.ToString())) {
////			WriteStat(FName(*LocalStats[i].Id), LocalStats[i].Progress);
//		}
//	}
//
//	//for (int32 i = 0; i < LocalLeaderboardStats.Num(); i++)
//	//{
//	//	if (LocalLeaderboardStats[i].Id.Equals(Stat.ToString())) {
//	//		WriteLeaderboardStat(SessionName, FName(*LocalStats[i].Id), LocalStats[i].Progress);
//	//	}
//	//}
//}


//void USteamStatsInfo::InitStats_Implementation(ULocalPlayer* LPlayer, FName SessionNameParam)
//{
//	//UE_LOG(LogShooter, Warning, TEXT("InitStats_Implementation"));
//	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("InitStats_Implementation"));
//	LocalPlayer = LPlayer;
//	SessionName = SessionNameParam;
//	TArray<FString> Array;
//	Array.SetNumUninitialized(0);
//	LocalStats.SetNumUninitialized(0);
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESteamStats"), true);
//	for (int32 i = 0; i < EnumPtr->NumEnums(); i++) {
//		FString Nombre = EnumPtr->GetEnumName(i);
//		Nombre.RemoveAt(0, 3,true);
//
//		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("InitStats_Implementation: %s"), *Nombre));
//		Array.Add(Nombre);
//	}
//
//	for (int32 i = 0; i < Array.Num(); i++)
//	{
//		FLocalOnlineStat Stat;
//		Stat.Id = Array[i];
//		Stat.Progress = 0;
//		LocalStats.Add(Stat);
//	}
//
//
//	TArray<FString> ArrayLeaderboardsStats;
//	ArrayLeaderboardsStats.SetNumUninitialized(0);
//
//	ArrayLeaderboardsStats.Add("ELO");
//
//	for (int32 i = 0; i < ArrayLeaderboardsStats.Num(); i++)
//	{
//		FLocalOnlineStat Stat;
//		Stat.Id = ArrayLeaderboardsStats[i];
//		Stat.Progress = 0;
////		LocalLeaderboardStats.Add(Stat);
//	}
//
////	QueryAchievements(2);
////	GetCachedSteamStats(true, Array);
////}
//
//void USteamStatsInfo::FillStatsWithSteamStats_Implementation()
//{
//	TArray<FString> Names;
//	Names.SetNumUninitialized(0);
//
//	for (int32 i = 0; i < LocalStats.Num(); i++)
//	{
//		Names.Add(LocalStats[i].Id);
//	}
//
////	GetCachedSteamStats(true, Names);
//
//	for (int32 i = 0; i < OutStats.Num(); i++)
//	{
//		LocalStats[i].FillWithOnlineStat(OutStats[i]);
//	}
//}
//
//bool USteamStatsInfo::FillStatsWithSteamStats_Validate() {
//	return true;
//}
//
//
//float USteamStatsInfo::GetStatValue(FName Name)
//{
//	FLocalOnlineStat Stat = GetStat(Name);
//	if (Stat.Id != "NULL")
//		return Stat.Progress;
//	else
//		return -1.0f;
//	return -1.0f;
//}
//
//FString USteamStatsInfo::GetStatInfo(FName Name)
//{
//	FLocalOnlineStat Stat = GetStat(Name);
//	if (Stat.Id != "NULL")
//		return Stat.ToDebugString();//Stat->GetStatInfo();
//	else
//		return "ERROR: stat not found";
//	return "";
//}
//
//FLocalOnlineStat USteamStatsInfo::GetStat(FName Name)
//{
//	FLocalOnlineStat Stat;
//	Stat.Id = "NULL";
//
//	for (int32 i = 0; i < LocalStats.Num(); i++)
//	{
//		//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, FString::Printf(TEXT("Stat: %s - Progress: %f"), *LocalStats[i].Id, LocalStats[i].Progress));
//		if (LocalStats[i].Id.Equals(Name.ToString()))
//			return LocalStats[i];
//	}
//
//	return Stat;
//}
//
//void USteamStatsInfo::WriteLocalStat(FName StatName, float Value)
//{
//	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("USteamStatsInfo WriteLocalStat: %s - %f"), *StatName.ToString(), Value));
//	for (int32 i = 0; i < LocalStats.Num(); i++){
//		//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("USteamStatsInfo WriteLocalStat2: %s - %f"), *LocalStats[i].Id, Value));
//		if (LocalStats[i].Id.Equals(StatName.ToString())) {
//			//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("USteamStatsInfo WriteLocalStat3: %s - %f"), *LocalStats[i].Id, Value));
//			//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("USteamStatsInfo WriteLocalStat3: %s - %f"), *LocalStats[i].Id, Value));
//			LocalStats[i].Progress = Value;
//		}
//	}
//}
//
//void USteamStatsInfo::IncrementLocalStat(FName Name, float Increment)
//{
//	for (int32 i = 0; i < LocalStats.Num(); i++)
//	{
//		if (LocalStats[i].Id.Equals(Name.ToString()))
//			LocalStats[i].Progress += Increment;
//	}
//}
//
//void USteamStatsInfo::UpdateAllStatsToSteam()
//{
//	for (int32 i = 0; i < LocalStats.Num(); i++)
//	{
////		WriteStat(FName(*LocalStats[i].Id), LocalStats[i].Progress);
//
//		//for (int32 j = 0; j < LocalLeaderboardStats.Num(); j++)
//		//{
//		//	if (LocalLeaderboardStats[j].Id.Equals(*LocalStats[i].Id)) {
//		//		WriteLeaderboardStat(SessionName, FName(*LocalStats[i].Id), LocalStats[i].Progress);
//		//		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("UpdateAllStatsToSteam WriteLeaderboardStat: %s - %f"), *LocalStats[i].Id, LocalStats[i].Progress));
//		//	}
//		//}
//	}
//}
//
//void USteamStatsInfo::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	// Replicate to everyone
//	DOREPLIFETIME(USteamStatsInfo, LocalStats);
//	/*DOREPLIFETIME(USteamStatsInfo, LocalLeaderboardStats);*/
//	DOREPLIFETIME(USteamStatsInfo, SessionName);
//
//}
//
////bool USteamStatsInfo::QueryAchievements_Validate(int32 Value) { return true; }
//bool USteamStatsInfo::InitStats_Validate(ULocalPlayer* LPlayer, FName SessionNameParam) { return true; }
////bool USteamStatsInfo::WriteStat_Validate(FName StatName, float Value) { return true; }
////bool USteamStatsInfo::WriteLeaderboardStat_Validate(FName SessionName, FName StatName, float Value) { return true; }
////bool USteamStatsInfo::GetAllUserValuesOfLeaderboard_Validate(FName NameOfLeaderboard) { return true; }
