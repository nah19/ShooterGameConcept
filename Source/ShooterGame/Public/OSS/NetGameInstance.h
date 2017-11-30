

#pragma once

#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "Engine.h"
#include "Core.h"
#include "ShooterGame.h"
#include "OnlineSubsystemUtils.h"
#include "Classes/FindSessionsCallbackProxy.h"
#include "OnlineSessionSettings.h"

#include "OnlineLeaderboardInterface.h"
#include "Steam/SteamStat.h"
#include "OnlineAchievementsInterface.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "NetworkReplayStreaming.h"
#include "NetGameInstance.generated.h"

class AConceptPlayerController;
class UConceptUserCloudStats;

USTRUCT(BlueprintType)
struct FPlayerPersistentData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		int32 UniqueId;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		FString Gametag;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		UClass* ClassToSpawn;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		TArray<UClass*> AbilitiesToSpawn;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		int32 Team;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		bool Ready;

		FOnlineSessionSearchResult PersistentSession;

	FPlayerPersistentData()
	{
		ClassToSpawn = NULL;
		Gametag = "ghgkijgyakgyud";
		UniqueId = -1;
		Team = -1;
		Ready = false;
		AbilitiesToSpawn.SetNumUninitialized(5);
		AbilitiesToSpawn[0] = NULL;
		AbilitiesToSpawn[1] = NULL;
		AbilitiesToSpawn[2] = NULL;
		AbilitiesToSpawn[3] = NULL;
		AbilitiesToSpawn[4] = NULL;
		PersistentSession = FOnlineSessionSearchResult();
	}
};

USTRUCT(BlueprintType)
struct FS_ReplayInfo
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString ReplayName;
    
    UPROPERTY(BlueprintReadOnly)
    FString FriendlyName;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;
    
    UPROPERTY(BlueprintReadOnly)
    int32 LengthInMS;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsValid;
    
    FS_ReplayInfo(FString NewName, FString NewFriendlyName, FDateTime NewTimestamp, int32 NewLengthInMS)
    {
        ReplayName = NewName;
        FriendlyName = NewFriendlyName;
        Timestamp = NewTimestamp;
        LengthInMS = NewLengthInMS;
        bIsValid = true;
    }
    
    FS_ReplayInfo()
    {
        ReplayName = "Replay";
        FriendlyName = "Replay";
        Timestamp = FDateTime::MinValue();
        LengthInMS = 0;
        bIsValid = false;
    }
};

USTRUCT(BlueprintType)
struct FPlayerStat
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		FString StatName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		int32 Value;

	FPlayerStat()
	{
		StatName = "";
		Value = 0;
	}
};

USTRUCT(BlueprintType)
struct FLocalOnlineAchievement
{

	GENERATED_USTRUCT_BODY()
	/** The id of the achievement */
		UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FLocalOnlineAchievement")
		FString Id;

	/** The progress towards completing this achievement: 0.0-100.0 */
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FLocalOnlineAchievement")
	int32 Progress;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FLocalOnlineAchievement")
	bool bFoundID;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FLocalOnlineAchievement")
	FText Title;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FLocalOnlineAchievement")
	FText LockedDescription;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FLocalOnlineAchievement")
	FText UnlockedDescription;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FLocalOnlineAchievement")
		bool bHidden;

	/** Returns debugging string to print out achievement info */
	FString ToDebugString() const
	{
		return FString::Printf(TEXT("Id='%s', Progress=%d"),
			*Id,
			Progress
		);
	}
};


USTRUCT(BlueprintType)
struct FGamePersistentData
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		TArray<UClass*> Team1PlayersClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		TArray<UClass*> Team2PlayersClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		TArray<FString> Team1Names;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		TArray<FString> Team2Names;

	//FGamePersistentData()
	//{
	//	Team1PlayersClass.SetNumUninitialized(5);
	//	Team2PlayersClass.SetNumUninitialized(5);
	//	Team1Names.SetNumUninitialized(5);
	//	Team2Names.SetNumUninitialized(5);
	//}
};

USTRUCT(BlueprintType)
struct FLeaderboardRowConcept
{
	GENERATED_BODY()

		UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		float StatValue;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		int32 Rank;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		FString UserName;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		FString AvatarUrl;
	//UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "Ability")
		TSharedPtr<const FUniqueNetId> PlayerID;

	//Constructor
		FLeaderboardRowConcept()
	{
		StatValue = 0;
		UserName = "EMPTY";
		Rank = -1;
	}
};

//USTRUCT()
//struct FLocalOnlineStat
//{
//	GENERATED_BODY()
//		/** The id of the achievement */
//		FString Id;
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


UCLASS()
class SHOOTERGAME_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
		//void Init() override;
	FOnLeaderboardReadCompleteDelegate LeaderboardReadCompleteDelegate;

	FDelegateHandle LeaderboardReadCompleteDelegateHandle;

	FOnlineLeaderboardRead* LeaderboardReadObject;

	TArray<FCloudFileHeader> FileHearders;
	
	TArray<uint8> FileContentsGlobal;

	FSocket* InternetSocket;

	//UPROPERTY(Replicated)
	//	TArray<FLocalOnlineStat> LocalLeaderboardStats;
	UPROPERTY(VisibleAnywhere, Blueprintreadonly, category = "Steamstats")
		TArray<FLeaderboardRowConcept> LeaderboardReadeableData;
	UPROPERTY(VisibleAnywhere, Blueprintreadonly, category = "Steamstats")
		TArray<UTexture2D*> LeaderboardReadeableTextures;

	UFUNCTION(/*server, reliable, withvalidation, */BlueprintCallable, Category = "Online")
		void GetAllUserValuesOfLeaderboard(FName NameOfLeaderboard);

	UFUNCTION(/*server, reliable, withvalidation, */BlueprintCallable, Category = "Online")
		void ReadLeaderboards(FName NameOfLeaderboard);

	void OnCompleteLeaderboard(bool bIsSuccess/*FOnlineLeaderboardReadRef Leaderboard*/);

	void OnReadCompleteLeaderboard(bool bIsSuccess);

	void OnCompleteQueryUserInfo(int32 UserIndex, bool QuerySuccess, const TArray< TSharedRef<const FUniqueNetId> >& UserNetIds, const FString& ErrorString);

	UFUNCTION(BlueprintCallable, Category = "Online")
		void WriteLeaderboardStat(FName StatName, float Value);

	UPROPERTY()
		bool bIsUsingGamepad;

		UFUNCTION()
		void SessionInviteAccepted(const FBlueprintSessionResult& InviteResult);

		UNetGameInstance(const FObjectInitializer& ObjectInitializer);

		UFUNCTION(Blueprintcallable, Category = "GameInstance")
			FPlayerPersistentData SetSessionPersistent(FPlayerPersistentData PlayerData, TArray<UClass*> AbilitiesToSpawn, UClass* ClassToSpawn, bool Ready);

		UFUNCTION(Blueprintcallable, Category = "GameInstance")
			void InitUserCloudStats();

		UFUNCTION()
			void OnInviteRequestAccepted();



		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
			UConceptUserCloudStats* UserCloudStats;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
			TSubclassOf<class UConceptUserCloudStats> UserCloudStatsClass;

		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		TArray<FPlayerPersistentData> PlayersData;

		int32 GetIndexOfPlayerData(APlayerState* PlayerState);

		UFUNCTION(Blueprintcallable, Category = "GameInstance")
		void ServerTravelTo(FString MapName);

		UFUNCTION(Blueprintcallable, Category = "GameInstance")
		void JOINIPGAME(FString IP);

		UFUNCTION(Blueprintcallable, Category = "GameInstance")
			void StopSearch();

		UFUNCTION(BlueprintImplementableEvent, Category = "PlayerAchievements")
			void SetAchievement(const AConceptPlayerController* PlayerController, const FString &AchievementName, const float Progress);

		UFUNCTION(BlueprintImplementableEvent, Category = "PlayerStats")
			void SetLeaderboardStat(const AConceptPlayerController* PlayerController, const USteamStat* Stat);

		UFUNCTION(Blueprintcallable, Category = "GameInstance")
			bool SaveFileToUserCloud(FString saveName, APlayerController* Controller);
		
		UFUNCTION(Blueprintcallable, Category = "GameInstance")
			bool ReadFileOfUserCloud(FString saveName, APlayerController* Controller);
		
		UFUNCTION(Blueprintcallable, Category = "GameInstance")
			bool GetFileOfUserCloud(FString saveName, APlayerController* Controller);
		
			void OnWriteUserFileComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName);
		
			void OnReadUserFileComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName);





			UFUNCTION(Blueprintcallable, Category = "GameInstance")
				bool SaveFiles(APlayerController* Controller);

			UFUNCTION(Blueprintcallable, Category = "GameInstance")
				bool ReadFiles(APlayerController* Controller);

			UFUNCTION(Blueprintcallable, Category = "GameInstance")
				bool GetFiles(APlayerController* Controller);
		//UFUNCTION(BlueprintImplementableEvent, Category = "PlayerStats")
		//	void GetLeaderboardStat(const AConceptPlayerController* PlayerController, const USteamStat* &Stat);

		//UFUNCTION()
		//	TArray<FOnlineAchievement>* GetAchievements();

		TArray<FOnlineAchievement> Achievements;

		TArray<FOnlineAchievement> GetAchievements();


		UFUNCTION(BlueprintImplementableEvent, Category = "PlayerStats")
			void GetPlayerStat(const AConceptPlayerController* PlayerController, const USteamStat* Stat);

		UFUNCTION(BlueprintImplementableEvent, Category = "PlayerStats")
			void SetPlayerStat(const AConceptPlayerController* PlayerController, const USteamStat* Stat);


		//NEW CREATESESSION C++ SYSTEM

		FOnlineSessionSearchResult CurrentSessionSearched;
		FOnlineSession CurrentSession;

		TSharedPtr<class FOnlineSessionSettings> SessionSettings;

		/* Delegate called when session created */
		FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
		/* Delegate called when session started */
		FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

		FOnReadUserFileCompleteDelegate OnReadUserFileCompleteDelegate;
		FOnWriteUserFileCompleteDelegate OnWriteUserFileCompleteDelegate;

		/** Handles to registered delegates for creating a session */
		FDelegateHandle OnCreateSessionCompleteDelegateHandle;
		FDelegateHandle OnStartSessionCompleteDelegateHandle;

		bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
		virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
		void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			void StartOnlineGame(bool bIsLAN, bool bIsPresence, FString Map);

		//NEW FINDSESSIONS C++ SYSTEM

		TSharedPtr<class FOnlineSessionSearch> SessionSearch;

		FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
		FDelegateHandle OnFindSessionsCompleteDelegateHandle;

		void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence);
		void OnFindSessionsComplete(bool bWasSuccessful);
    FString TypeOfGameMap;
	
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void FindOnlineGames(bool bIsLAN, bool bIsPresence, FString GameMap);

		FString MapName;
		bool LanGame = false;
		bool PresenceGame;
		TMap<int32, FOnlineSessionSearchResult> AvailableSessions;

		//Continuous searcher
		void SearchSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence);
		void OnSearchSessionsComplete(bool bWasSuccessful);
		UPROPERTY(BlueprintReadWrite, Category = "Network")
			int32 TotalSessions;

		UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "Network")
			bool bIsJoinning = false;

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void SearchOnlineGames();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
			void CheckControllers();

		FOnFindSessionsCompleteDelegate OnSearchSessionsCompleteDelegate;
		FDelegateHandle OnSearchSessionsCompleteDelegateHandle;

		//NEW JOIN GAME C++ SYSTEM

		bool JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

		void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

		/** Delegate for joining a session */
		FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

		/** Handle to registered delegate for joining a session */
		FDelegateHandle OnJoinSessionCompleteDelegateHandle;

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			void JoinOnlineGame();

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			static FString GetGameVersion();

		//Destroy system

		FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
		FDelegateHandle OnDestroySessionCompleteDelegateHandle;
    
        FOnEndSessionCompleteDelegate OnEndSessionCompleteDelegate;
        FDelegateHandle OnEndSessionCompleteDelegateHandle;

        virtual void OnEndSessionComplete(FName SessionName, bool bWasSuccessful);
    
		virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			void DestroySessionAndLeaveGame();
        UFUNCTION(BlueprintCallable, Category = "Network|Test")
            void CleanSession();
		void DisconnectAllClients();

		//Update Session
		
		FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;
		FDelegateHandle OnUpdateSessionCompleteDelegateHandle;
		
		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			bool UpdateSessionMode(FString NewMode);

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
		UTexture2D* SteamAvatar(bool& IsValid);

		UFUNCTION()
			UTexture2D* SteamFriendAvatar(uint64 IDSteam);

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			TArray<UTexture2D*> SteamLeaderboardFriendAvatar(ASteamPlayerState* PlayerStateParam);

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			UTexture2D* SteamAchievementIcon(FString AchievementName);

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			UTexture2D* ConvertIntToTexture(int Picture);

		void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);

		//Reconect

		UFUNCTION(BlueprintCallable, Category = "Network|Test")
			bool ReConnectOnlineGame(FPlayerPersistentData PlayerData);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		int32 attempts = 0;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		bool ShouldFindSessions = false;
    
    //Replay
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StartRecReplay();
    
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StopRecReplay();
    
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void PlayRecReplay(FString RecName);
    
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void FindReplays();
    
    // for FindReplays()
    TSharedPtr<INetworkReplayStreamer> EnumerateStreamsPtr;
    FOnEnumerateStreamsComplete OnEnumerateStreamsCompleteDelegate;
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Replay")
    void BP_OnFindReplaysComplete(const TArray<FS_ReplayInfo> &AllReplays);
    
    void OnEnumerateStreamsComplete(const TArray<FNetworkReplayStreamInfo>& StreamInfos);
    

    virtual void PlayReplay(const FString& InName, UWorld* WorldOverride = nullptr, const TArray<FString>& AdditionalOptions = TArray<FString>()) override;
    
    void HandlePreLoadMap(const FString& MapName);
    
    FDelegateHandle _HandlePreLoadMap;

public:
	
	TSharedPtr<const FUniqueNetId> PlayerId;

	UPROPERTY()
		int32 LocalIndexForPlayer;

	// Internal callback when session creation completes, calls StartSession
	//void OnSessionUserInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult);

	FOnSessionUserInviteAcceptedDelegate OnSessionInviteAcceptedDelegate;
	FDelegateHandle OnSessionInviteAcceptedDelegate_Handle;

	FOnSessionUserInviteAcceptedDelegate OnSessionUserInviteAcceptedDelegate;
	FDelegateHandle OnSessionUserInviteAcceptedDelegate_Handle;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegate_Handle;

	virtual void Shutdown() override;
	

	virtual void Init() override;

	void OnSessionInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult);

	//void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


	//void NewStat(FString Name);

	//TArray<FPlayerStat*> PlayerStats;
    
    UFUNCTION(BlueprintCallable, category ="Network")
    void InternetConnectionChecker();
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, category ="Network")
    bool ConnectedToInternet;
    FTimerHandle ConnectionHandle;
    
	UFUNCTION(BlueprintCallable, Category = "Instance Online|Achievements")
	void CacheAchievements(class APlayerController* PlayerController);

	void OnCacheAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);

	TArray<FOnlineAchievement> CachedAchievementsEarly;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, category = "Network")
	TArray<FLocalOnlineAchievement> CachedAchievements;

	UFUNCTION(BlueprintCallable, Category = "Instance Online|Achievements")
	void GetCachedAchievements();

	void OnWriteAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Instance Online|Achievements")
	void WriteAchievements(TArray<FName> AchievementsKeys, TArray<float> AchievementsValues);

	UFUNCTION(BlueprintCallable, Category = "Instance Online|Achievements")
	void ResetStats();


};


class FInternetChecker : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FInternetChecker>;

public:
	FInternetChecker(bool &InternetVar) :
		Internet(InternetVar)
	{}

protected:
	bool& Internet;

	void DoWork()
	{
		//if (!InternetSocket) {
		FSocket* InternetSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
		//}

		FString address = TEXT("172.217.24.110");
		int32 port = 80;
		FIPv4Address ip;

		FIPv4Address::Parse(address, ip);

		TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		addr->SetIp(ip.Value);
		addr->SetPort(port);

		if (InternetSocket) {
			Internet = InternetSocket->Connect(*addr);
		}
	}

	// This next section of code needs to be here.  Not important as to why.

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FInternetChecker, STATGROUP_ThreadPoolAsyncTasks);
	}
};
