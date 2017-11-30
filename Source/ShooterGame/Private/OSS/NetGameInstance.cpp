
#include "ShooterGame.h"
#include "Engine.h"
#include "NetGameInstance.h"
#include "Steam/SteamStatsInfo.h"
#include "GM_MainMenu.h"
#pragma push_macro("ARRAY_COUNT")
#undef ARRAY_COUNT


#pragma pop_macro("ARRAY_COUNT")

#include "ConceptPlayerController.h"

#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Steam/steam_api.h"
#include "Steam/isteamuser.h"
#include "Steam/isteamuserstats.h"
#endif
#include "OnlineSubsystemUtils.h"
#include "Runtime/NetworkReplayStreaming/NullNetworkReplayStreaming/Public/NullNetworkReplayStreaming.h"
#include "NetworkVersion.h"
#include "ConceptUserCloudStats.h"
#include "OnlineSubsystemUtilsClasses.h"





UNetGameInstance::UNetGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	OnSessionUserInviteAcceptedDelegate = FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UNetGameInstance::OnSessionInviteAccepted);
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnStartOnlineGameComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnJoinSessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnFindSessionsComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnDestroySessionComplete);
    OnEndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnEndSessionComplete);
	OnUpdateSessionCompleteDelegate = FOnUpdateSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnUpdateSessionComplete);
	OnSearchSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnSearchSessionsComplete);
	OnWriteUserFileCompleteDelegate = FOnWriteUserFileCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnWriteUserFileComplete);
	OnReadUserFileCompleteDelegate = FOnReadUserFileCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnReadUserFileComplete);

	FString test = "";
	LocalIndexForPlayer = 0;
	ShouldFindSessions = false;

	bIsUsingGamepad = false;

#if PLATFORM_PS4
	bIsUsingGamepad = true;
#endif
}

void UNetGameInstance::Init()
{
	Super::Init();

	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("INIT")));
	//InitUserCloudStats();

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();
		if (SessionInt.IsValid())
		{
			int32 ControllerId = 0;
			if (ControllerId != 255)
			{
				FOnSessionUserInviteAcceptedDelegate OnSessionInviteAcceptedDelegate = FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UNetGameInstance::OnSessionInviteAccepted);
				OnSessionUserInviteAcceptedDelegate_Handle = SessionInt->AddOnSessionUserInviteAcceptedDelegate_Handle(OnSessionInviteAcceptedDelegate);
			}
		}
	}
    // create a ReplayStreamer for FindReplays() and DeleteReplay(..)
    EnumerateStreamsPtr = FNetworkReplayStreaming::Get().GetFactory().CreateReplayStreamer();
    // Link FindReplays() delegate to function
    OnEnumerateStreamsCompleteDelegate = FOnEnumerateStreamsComplete::CreateUObject(this, &UNetGameInstance::OnEnumerateStreamsComplete);
}

void UNetGameInstance::InitUserCloudStats()
{
	FString UserCloudStatsName = FString(TEXT("usercloudstats"));

	if (!UserCloudStats) {
		APlayerController* LocalPlayerController = GetPrimaryPlayerController();
		
		if (!LocalPlayerController) { return; }

		if (!LocalPlayerController->IsPrimaryPlayer()) {
			UserCloudStats = NewObject<UConceptUserCloudStats>(this, UserCloudStatsClass);
			return;
		}
		else if (UserCloudStats) {
			return;
		}

		UserCloudStats = Cast<UConceptUserCloudStats>(UGameplayStatics::LoadGameFromSlot(UserCloudStatsName, 0));
		
		ReadFileOfUserCloud(UserCloudStatsName, LocalPlayerController);
	}
}

void UNetGameInstance::OnSessionInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult)
{
	//UE_LOG(LogShooter, Verbose, TEXT("OnSessionInviteAccepted LocalUserNum: %d bSuccess: %d"), LocalUserNum, bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::White, FString::Printf(TEXT("-------- OnSessionUserInviteAccepted -------------")));
	if (bWasSuccessful)
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if (OnlineSub) {
			IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
			if (Sessions.IsValid()) {
				CheckControllers();
				Sessions->ClearOnSessionUserInviteAcceptedDelegate_Handle(OnSessionUserInviteAcceptedDelegate_Handle);
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::White, FString::Printf(TEXT("-------- OnSessionUserInviteAccepted ------ AND JOINING -------")));
				const FBlueprintSessionResult& Result = FBlueprintSessionResult();
				FOnlineSessionSearchResult RResult = SearchResult;
				this->SessionInviteAccepted(Result);
				ULocalPlayer* const Player = GetFirstGamePlayer();
				OnInviteRequestAccepted();
				JoinOnlineSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);
				PlayerId = UserId;
			}
		}
	}
}

//void UNetGameInstance::OnSessionUserInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult)
//{
//	//UE_LOG(LogShooter, Warning, TEXT("OnSessionUserInviteAccepted"));
//	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("OnSessionUserInviteAccepted")));
//	if (bWasSuccessful)
//	{
//
//		const FBlueprintSessionResult& Result = FBlueprintSessionResult();
//		FOnlineSessionSearchResult RResult = SearchResult;
//		this->SessionInviteAccepted(Result);
//		ULocalPlayer* const Player = GetFirstGamePlayer();
//		OnInviteRequestAccepted();
//		JoinOnlineSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);
//		PlayerId = UserId;
//	}
//	else
//	{
//		FFrame::KismetExecutionMessage(TEXT("FUUU Invite not succesful"), ELogVerbosity::Warning);
//	}
//
//	//UE_LOG(LogShooter, Warning, TEXT("OnSessionUserInviteAcceptedEnd"));
//	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("OnSessionUserInviteAcceptedEnd")));
//}

//void UNetGameInstance::Init() {
//	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("BeginPlay")));
//	SessionInviteAcceptedDelegate = FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UNetGameInstance::OnSessionUserInviteAccepted);
//	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnCreateSessionComplete);
//	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnStartOnlineGameComplete);
//	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnJoinSessionComplete);
//	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnFindSessionsComplete);
//	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnDestroySessionComplete);
//	OnUpdateSessionCompleteDelegate = FOnUpdateSessionCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnUpdateSessionComplete);
//	OnSearchSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnSearchSessionsComplete);
//}

void UNetGameInstance::OnInviteRequestAccepted() {
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("OnInviteRequestAccepted_Implementation")));
}

void UNetGameInstance::SessionInviteAccepted(const FBlueprintSessionResult &InviteResult)
{
	//UE_LOG(LogShooter, Warning, TEXT("SessionInviteAccepted_Implementation"));
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("SessionInviteAccepted_Implementation")));
}

//NEW C++ session system

bool UNetGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers) {
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("HostSession")));

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("HostSession OnlineSub")));
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid()) {
			PlayerId = UserId;

			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 10;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			FString GameMode = MapName + GetGameVersion();
			FString OnlySurvival = "Lobby";
			if (MapName == "Survival") {
				OnlySurvival = "Survival";
				MapName = "Lobby";
			}
			SessionSettings->Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService);
			SessionSettings->Set(SETTING_GAMEMODE, GameMode, EOnlineDataAdvertisementType::ViaOnlineService);
			//SessionSettings->Set(SETTING_CUSTOMSEARCHINT1, OnlySurvival, EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("HostSession end")));
#if PLATFORM_PS4
			if (!ConnectedToInternet) {
				UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName), true, "listen");
				return false;
			}
#endif
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	} else {
		UE_LOG(LogShooter, Warning, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void UNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete")));
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid()) {
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful) {
				
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				Sessions->StartSession(SessionName);
				GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("StartSession")));
			}
			else {
				bIsJoinning = false;
			}
		}
	}
}

void UNetGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful) {

	if(bWasSuccessful){
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("OnStartOnlineGameComplete succes")));
	}
	else{
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("OnStartOnlineGameComplete FAIL")));
	}
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid()) {
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful) {
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName), true, "listen");
	}
}

void UNetGameInstance::StartOnlineGame(bool bIsLAN, bool bIsPresence, FString Map) {
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("StartOnlineGame")));
	MapName = Map;
	attempts = 0;
	HostSession(GetFirstGamePlayer()->GetPreferredUniqueNetId(), GameSessionName, bIsLAN, bIsPresence, 10);
}

void UNetGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("FindSessions")));
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid()) {

			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			if (bIsPresence) {
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}
			const TSharedRef< FOnlineSessionSearch > SearchSettingsRef = SessionSearch.ToSharedRef();

			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			Sessions->FindSessions(0, SearchSettingsRef);
			
			PlayerId = UserId;
		}
	} else {
		OnFindSessionsComplete(false);
	}
}

void UNetGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {
	if(bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("OnFindSessionsComplete bWasSuccessful")));
	}

	ULocalPlayer* const Player = GetFirstGamePlayer();
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	AvailableSessions.Empty();

	if (OnlineSub && Player) {
		UE_LOG(LogShooter, Warning, TEXT("OnlineSub"));
		attempts++;
		UE_LOG(LogShooter, Warning, TEXT("Attemps: %d"), attempts);
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid()) {
			UE_LOG(LogShooter, Warning, TEXT("SessionValid"));
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			UE_LOG(LogShooter, Warning, TEXT("SearchResults: %d"), SessionSearch->SearchResults.Num());
			if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0) {

				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++) {
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("Session Number: %d | Sessionname: %s | Attempt %d"), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName), attempts));

					FString GameMode;
                    FString SessionGameMap;
					if (SessionSearch->SearchResults[SearchIdx].Session.OwningUserId != Player->GetPreferredUniqueNetId()) {
						UE_LOG(LogShooter, Warning, TEXT("OwningUserId"));
						if (SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(SETTING_GAMEMODE, GameMode)) {
							GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("OwningUserId: %s | GetPreferredUniqueNetId: %s"), *Player->GetPreferredUniqueNetId()->ToString(), *Player->GetPreferredUniqueNetId()->ToString()));
							GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::White, FString::Printf(TEXT("GameMode: %s"), *GameMode));
							GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::White, FString::Printf(TEXT("PING: %d"), SessionSearch->SearchResults[SearchIdx].PingInMs));
							if (GameMode == (TypeOfGameMap + GetGameVersion()) /*|| GameMode == ("Survival" + GetGameVersion()) || (attempts > 3 && GameMode.Contains(GetGameVersion()))*/) {
								AvailableSessions.Add(SessionSearch->SearchResults[SearchIdx].PingInMs, SessionSearch->SearchResults[SearchIdx]);
							}
						}
					}
					FString ToPrint;
					bool Impreso = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(SETTING_MAPNAME, ToPrint);
				}
				AvailableSessions.KeySort([](int32 A, int32 B) { return B < A; });
				for (auto& Session : AvailableSessions) {
					if (JoinOnlineSession(Player->GetPreferredUniqueNetId(), GameSessionName, Session.Value)) {
						bIsJoinning = true;
						attempts = 0;
						return;
					}
				}

				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("AvailableSessions: %d"), AvailableSessions.Num()));
			}
			if(attempts > 5){
				bIsJoinning = true;
				StartOnlineGame(LanGame, true, TypeOfGameMap);
				GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, FString::Printf(TEXT("StartOnlineGame")));
				return;
			}
			if(ShouldFindSessions){
				if (attempts <= 20000) {
					FindSessions(Player->GetPreferredUniqueNetId(), GameSessionName, SessionSearch->bIsLanQuery, true);
				} else {
					UGameplayStatics::OpenLevel(GetWorld(), "MenuOptimized", true);
				}
			}
		}
	}
}

void UNetGameInstance::FindOnlineGames(bool bIsLAN, bool bIsPresence, FString GameMap) {
#if PLATFORM_PS4

	if (!ConnectedToInternet) {
		UGameplayStatics::OpenLevel(GetWorld(), FName(*GameMap), true, "listen");
		return;
	}

#endif


	ULocalPlayer* const Player = GetFirstGamePlayer();
	LanGame = bIsLAN;
	PresenceGame = bIsPresence;
	ShouldFindSessions = true;
    TypeOfGameMap = GameMap;
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, FString::Printf(TEXT("FindOnlineGames")));
	FindSessions(Player->GetPreferredUniqueNetId(), GameSessionName, bIsLAN, bIsPresence);
}


void UNetGameInstance::SearchOnlineGames() { 
	ULocalPlayer* const Player = GetFirstGamePlayer();
	SearchSessions(Player->GetPreferredUniqueNetId(), GameSessionName, false, true);
}

void UNetGameInstance::SearchSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence) {
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid()) {

			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 500;
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);

			const TSharedRef< FOnlineSessionSearch > SearchSettingsRef = SessionSearch.ToSharedRef();
			OnSearchSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnSearchSessionsCompleteDelegate);
			Sessions->FindSessions(0, SearchSettingsRef);

			PlayerId = UserId;
		}
	}
	else {
		OnFindSessionsComplete(false);
	}
}

void UNetGameInstance::OnSearchSessionsComplete(bool bWasSuccessful) {
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid()) {
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnSearchSessionsCompleteDelegateHandle);
			if (bWasSuccessful) {
				TotalSessions = SessionSearch->SearchResults.Num();
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("num sessions %d"), TotalSessions));
			}
		}
	}
	
}


bool UNetGameInstance::JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult) {
	UE_LOG(LogShooter, Warning, TEXT("JoinOnlineSession"));
	bool bSuccessful = false;
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid()) {
			//Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
			if(!bSuccessful){
				bIsJoinning = false;
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("CANT JOIN SESSION"));
			}
			CurrentSessionSearched = SearchResult;
			CurrentSession = SearchResult.Session;
			PlayerId = UserId;
		}
	}

	return bSuccessful;
}

void UNetGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	UE_LOG(LogShooter, Warning, TEXT("OnJoinSessionComplete"));
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		
		if (Sessions.IsValid()) {
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			if(Result == EOnJoinSessionCompleteResult::Success) {
				APlayerController * const PlayerController = GetFirstLocalPlayerController();
				FString TravelURL;
				if (PlayerController) {
					if(Sessions->GetResolvedConnectString(SessionName, TravelURL)) {
						PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
						bIsJoinning = true;
					}
				}
			}
		}
	}
}

void UNetGameInstance::JoinOnlineGame() {
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FOnlineSessionSearchResult SearchResult;

	if (SessionSearch->SearchResults.Num() > 0) {
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++) {
			FString GameMode;
			if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId() && SessionSettings->Get(SETTING_GAMEMODE, GameMode)) {
				if(GameMode == "Lobby" + GetGameVersion()) {
					SearchResult = SessionSearch->SearchResults[i];

					JoinOnlineSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);
					break;
				}
			}
		}
	}
}

void UNetGameInstance::OnEndSessionComplete(FName SessionName, bool bWasSuccessful){
    UE_LOG(LogShooter, Warning, TEXT("Ending Session COMPLETE"));
    GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, TEXT("OnEndSessionComplete"));
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub) {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        
        if (Sessions.IsValid()) {
            
            UE_LOG(LogShooter, Warning, TEXT("Ending VALID Session"));
            Sessions->ClearOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegateHandle);
            if (bWasSuccessful) {
                UE_LOG(LogShooter, Warning, TEXT("Ending Session COMPLETE Successful"));
                GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, TEXT("OnEndSessionComplete Successful"));
            }
        }
    }
}

void UNetGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {

	UE_LOG(LogShooter, Warning, TEXT("Destroying Session COMPLETE"));
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, TEXT("OnDestroySessionComplete"));
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid()) {

			UE_LOG(LogShooter, Warning, TEXT("Destroying VALID Session"));
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			//if (bWasSuccessful) {
				//UE_LOG(LogShooter, Warning, TEXT("OPening Level"));

			UGameplayStatics::OpenLevel(GetWorld(), "MenuOptimized", true);
			//}
		}
	}
}

void UNetGameInstance::DestroySessionAndLeaveGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, TEXT("DestroySessionAndLeaveGame"));
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			if(OnlineSub->IsServer()) {
				StopRecReplay();
				OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
				OnEndSessionCompleteDelegateHandle = Sessions->AddOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegate);
				UE_LOG(LogShooter, Warning, TEXT("Destroying Session Server"));
				DisconnectAllClients();
				Sessions->EndSession(GameSessionName);
				Sessions->DestroySession(GameSessionName);
			} else {
				UE_LOG(LogShooter, Warning, TEXT("Destroying Session Client"));
				//Sessions->DestroySession(GameSessionName);
				Sessions->EndSession(GameSessionName);
				Sessions->DestroySession(GameSessionName);
				UGameplayStatics::OpenLevel(GetWorld(), "MenuOptimized", true);
			}
		}
	}
}

void UNetGameInstance::CleanSession(){
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        if (Sessions.IsValid()) {
            Sessions->EndSession(GameSessionName);
            Sessions->DestroySession(GameSessionName);
        }
    }
}

void UNetGameInstance::DisconnectAllClients() {
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		for (TActorIterator<APlayerController> Controllers(GetWorld()); Controllers; ++Controllers) {
			AConceptPlayerController* InGameController = Cast<AConceptPlayerController>(*Controllers);
			if (InGameController && !OnlineSub->IsServer()) {
				InGameController->OnLeaveGame(false);
			}
		}
	}
}

bool UNetGameInstance::UpdateSessionMode(FString NewMode){

	IOnlineSessionPtr Sessions = IOnlineSubsystem::Get()->GetSessionInterface();
	if (Sessions.IsValid())
	{
		FOnlineSessionSettings* CurrentSettings = Sessions->GetSessionSettings(GameSessionName);
		if (!CurrentSettings) return false;
		FOnlineSessionSettings NewSettings;
		NewSettings.bIsLANMatch = CurrentSettings->bIsLANMatch;
		NewSettings.bShouldAdvertise = CurrentSettings->bShouldAdvertise;
		NewSettings.bIsDedicated = CurrentSettings->bIsDedicated;
		NewSettings.bAllowJoinInProgress = CurrentSettings->bAllowJoinInProgress;
		NewSettings.bAllowInvites = CurrentSettings->bAllowInvites;
		NewSettings.NumPublicConnections = CurrentSettings->NumPublicConnections;
		NewSettings.NumPrivateConnections = CurrentSettings->NumPrivateConnections;
		NewSettings.bAllowJoinViaPresenceFriendsOnly = CurrentSettings->bAllowJoinViaPresenceFriendsOnly;
		NewSettings.bAllowJoinViaPresence = CurrentSettings->bAllowJoinViaPresence;
		NewSettings.bUsesPresence = CurrentSettings->bUsesPresence;
		NewSettings.bUsesStats = CurrentSettings->bUsesStats;
		NewSettings.bAntiCheatProtected = CurrentSettings->bAntiCheatProtected;
		NewSettings.BuildUniqueId = CurrentSettings->BuildUniqueId;

		FString Map;
		CurrentSettings->Get(SETTING_MAPNAME, Map);
		FString OnlySurvival;
		CurrentSettings->Get(SETTING_CUSTOMSEARCHINT1, OnlySurvival);
		//NewSettings.Set(SETTING_MAPNAME, Map);
		//NewSettings.Set(SETTING_GAMEMODE, NewMode);

		NewSettings.Set(SETTING_MAPNAME, Map, EOnlineDataAdvertisementType::ViaOnlineService);
		NewSettings.Set(SETTING_GAMEMODE, NewMode + GetGameVersion(), EOnlineDataAdvertisementType::ViaOnlineService);
		NewSettings.Set(SETTING_CUSTOMSEARCHINT1, OnlySurvival, EOnlineDataAdvertisementType::ViaOnlineService);
		OnUpdateSessionCompleteDelegateHandle = Sessions->AddOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegate);

		//UE_LOG(LogShooter, Warning, TEXT("NamePassed: %s"), *GameSessionName.ToString());
		if (Sessions->UpdateSession(GameSessionName, NewSettings))
		{
			FString ForPrint;
			CurrentSettings = Sessions->GetSessionSettings(GameSessionName);
			CurrentSettings->Get(SETTING_GAMEMODE, ForPrint);
			UE_LOG(LogShooter, Warning, TEXT("AfterUpdateGameMode: %s"), *ForPrint);
			return true;
		}
	}
	return false;
}

void UNetGameInstance::OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful) {
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid()) {
			Sessions->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
			if (bWasSuccessful) {
				FString ForPrint;
				FOnlineSessionSettings* CurrentSettings = Sessions->GetSessionSettings(GameSessionName);
				CurrentSettings->Get(SETTING_GAMEMODE, ForPrint);
				//UE_LOG(LogShooter, Warning, TEXT("OnUpdateSessionComplete: %s"), *GameSessionName.ToString());
			}
			else {
			}
		}
	}
}

bool UNetGameInstance::ReConnectOnlineGame(FPlayerPersistentData PlayerData) {
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FOnlineSessionSearchResult SearchResult = PlayerData.PersistentSession;
	//JoinSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);



	bool bSuccessful = false;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		TSharedPtr<const FUniqueNetId> UserId = Player->GetPreferredUniqueNetId();
		if (Sessions.IsValid() && UserId.IsValid()) {
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			bSuccessful = Sessions->JoinSession(*UserId, GameSessionName, SearchResult);
			CurrentSessionSearched = SearchResult;
			PlayerId = UserId;
		}
	}

	return bSuccessful;
}


int32 UNetGameInstance::GetIndexOfPlayerData(APlayerState* PlayerState){
	for (int32 i = 0; i < PlayersData.Num(); i++) {
		if (PlayersData[i].Gametag.Equals(PlayerState->PlayerName)){
			return i;
		}
	}
	return -1;
}

void UNetGameInstance::ServerTravelTo(FString MapName){
	GetWorld()->ServerTravel(MapName, false, false);
}

void UNetGameInstance::StopSearch() {
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid()) {
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			attempts = 0;
		}
	}
}

void UNetGameInstance::JOINIPGAME(FString Ip)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) {
		PC->ClientTravel(Ip + "?CHARACTER_NAME=Marcot", ETravelType::TRAVEL_Absolute, false, FGuid());
	}
}

FPlayerPersistentData UNetGameInstance::SetSessionPersistent(FPlayerPersistentData PlayerData, TArray<UClass*> AbilitiesToSpawn, UClass* ClassToSpawn, bool Ready) {
	PlayerData.PersistentSession = CurrentSessionSearched;
	PlayerData.AbilitiesToSpawn = AbilitiesToSpawn;
	PlayerData.ClassToSpawn = ClassToSpawn;
	PlayerData.Ready = Ready;

	return PlayerData;
}


bool UNetGameInstance::SaveFileToUserCloud(FString saveName, APlayerController* Controller)
{
	TArray<uint8> FileContents;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineUserCloudPtr UserCloud = OnlineSub->GetUserCloudInterface();
		if (UserCloud.IsValid())
		{
			UConceptUserCloudStats* UserCloudStatsLocal = Cast<UConceptUserCloudStats>(UGameplayStatics::LoadGameFromSlot(saveName, 0));
			if (UserCloudStatsLocal) {
				UserCloudStats = UserCloudStatsLocal;
			}
			UGameplayStatics::SaveGameToMemory(UserCloudStats, FileContents);

			ULocalPlayer* LocalPlayer = Controller->GetWorld()->GetFirstLocalPlayerFromController();
			TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();
			UserCloud->AddOnWriteUserFileCompleteDelegate_Handle(OnWriteUserFileCompleteDelegate);

			return UserCloud->WriteUserFile(*UniqueNetId, saveName, FileContents);
		}
	}
	//log error
	return false;
}


bool UNetGameInstance::SaveFiles(APlayerController* Controller)
{
	TArray<uint8> FileContents;

	uint8 int1 = 1;
	uint8 int3 = 3;
	uint8 int2 = 2;
	FileContents.Add(int1);
	FileContents.Add(int3);
	FileContents.Add(int2);

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineUserCloudPtr UserCloud = OnlineSub->GetUserCloudInterface();
		if (UserCloud.IsValid())
		{
			FString Prueba = FString(TEXT("prueba"));
			ULocalPlayer* LocalPlayer = Controller->GetWorld()->GetFirstLocalPlayerFromController();
			TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();
			UserCloud->AddOnWriteUserFileCompleteDelegate_Handle(OnWriteUserFileCompleteDelegate);
			return UserCloud->WriteUserFile(*UniqueNetId, Prueba, FileContents);
		}
	}
	//log error
	return false;
}

void UNetGameInstance::OnWriteUserFileComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName) {
	if (bWasSuccessful) {
	}
	else {
	}
}

bool UNetGameInstance::ReadFiles(APlayerController* Controller)
{
	TArray<uint8> FileContents;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineUserCloudPtr UserCloud = OnlineSub->GetUserCloudInterface();
		if (UserCloud.IsValid())
		{
			FString Prueba = FString(TEXT("prueba"));
			ULocalPlayer* LocalPlayer = Controller->GetWorld()->GetFirstLocalPlayerFromController();
			TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();
			UserCloud->AddOnReadUserFileCompleteDelegate_Handle(OnReadUserFileCompleteDelegate);
			return UserCloud->ReadUserFile(*UniqueNetId, Prueba);
		}
	}
	
	return false;
}



bool UNetGameInstance::ReadFileOfUserCloud(FString saveName, APlayerController* Controller)
{
	TArray<uint8> FileContents;
	FileContentsGlobal.Empty();
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineUserCloudPtr UserCloud = OnlineSub->GetUserCloudInterface();
		if (UserCloud.IsValid())
		{
			ULocalPlayer* LocalPlayer = Controller->GetWorld()->GetFirstLocalPlayerFromController();
			TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();
			UserCloud->AddOnReadUserFileCompleteDelegate_Handle(OnReadUserFileCompleteDelegate);
			return UserCloud->ReadUserFile(*UniqueNetId, saveName);
		}
	}

	return false;
}


bool UNetGameInstance::GetFileOfUserCloud(FString saveName, APlayerController* Controller)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineUserCloudPtr UserCloud = OnlineSub->GetUserCloudInterface();
		if (UserCloud.IsValid())
		{
			ULocalPlayer* LocalPlayer = Controller->GetWorld()->GetFirstLocalPlayerFromController();
			TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();
			bool bIsSuccess = UserCloud->GetFileContents(*UniqueNetId, saveName, FileContentsGlobal);

			if (bIsSuccess) {
				if (saveName.Equals(saveName)) {
					UGameplayStatics::SaveDataToSlot(FileContentsGlobal, saveName, 0);
					UserCloudStats = Cast<UConceptUserCloudStats>(UGameplayStatics::LoadGameFromSlot(saveName, 0));
				}
			}
			else {
				if (!UserCloudStats) {
					UserCloudStats = NewObject<UConceptUserCloudStats>(this, UserCloudStatsClass);
					UGameplayStatics::SaveGameToSlot(Cast<USaveGame>(UserCloudStats), saveName, 0);
				}
			}

			return bIsSuccess;
			//return true;
		}
	}
	//log error
	return false;
}

bool UNetGameInstance::GetFiles(APlayerController* Controller)
{
	

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineUserCloudPtr UserCloud = OnlineSub->GetUserCloudInterface();
		if (UserCloud.IsValid())
		{
			FString Prueba = FString(TEXT("prueba"));
			ULocalPlayer* LocalPlayer = Controller->GetWorld()->GetFirstLocalPlayerFromController();
			TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();
			bool bIsSuccess = UserCloud->GetFileContents(*UniqueNetId, Prueba, FileContentsGlobal);

			for (int32 i = 0; i < FileContentsGlobal.Num(); i++) {
			}

			return bIsSuccess;
			//return true;
		}
	}
	//log error
	return false;
}

void UNetGameInstance::OnReadUserFileComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName) {
	if (bWasSuccessful) {
		GetFileOfUserCloud(FileName, GetFirstLocalPlayerController());
	}
	else {
		if (!UserCloudStats) {
			UserCloudStats = NewObject<UConceptUserCloudStats>(this, UserCloudStatsClass);
			UGameplayStatics::SaveGameToSlot(Cast<USaveGame>(UserCloudStats), FileName, 0);
		}
	}
}


void UNetGameInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UNetGameInstance, PlayersData);
	//DOREPLIFETIME(UNetGameInstance, LocalLeaderboardStats);
}

void UNetGameInstance::Shutdown() {
	UE_LOG(LogShooter, Warning, TEXT("Game Shutdowns NOW!"));
	ABasePlayerController* InGameController = Cast<ABasePlayerController>(GetFirstLocalPlayerController());
	if (InGameController) {
		UE_LOG(LogShooter, Warning, TEXT("Shutdown if"));
		InGameController->OnLeaveGame();
	}
	//DestroySessionAndLeaveGame();
		
	Super::Shutdown();
}


TArray<FOnlineAchievement> UNetGameInstance::GetAchievements()
{
	auto pOnlineSubsystem = IOnlineSubsystem::Get();
	TArray<FOnlineAchievement> Achievements;
	
	const FUniqueNetId* LocalPlayer = GetLocalPlayerByIndex(0)->GetPreferredUniqueNetId().Get();
	pOnlineSubsystem->GetAchievementsInterface()->GetCachedAchievements(*LocalPlayer, Achievements);

	return Achievements;
}

FString UNetGameInstance::GetGameVersion(){
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);
	return ProjectVersion;
}


UTexture2D* UNetGameInstance::SteamAvatar(bool& IsValid)
{
    #if PLATFORM_MAC || PLATFORM_WINDOWS
	if (SteamAPI_Init()){
		//CSteamID davoID = CSteamID((uint64)76561197995724184);
		int Picture = SteamFriends()->GetMediumFriendAvatar(SteamUser()->GetSteamID());
		IsValid = true;

		return ConvertIntToTexture(Picture);
	}
    #endif
	IsValid = false;
	return nullptr;
}

TArray<UTexture2D*> UNetGameInstance::SteamLeaderboardFriendAvatar(ASteamPlayerState* PlayerStateParam) {
	
	TArray<FLeaderboardRowConcept> Leaderboard = LeaderboardReadeableData;
	TArray<UTexture2D*> ArrayTextures;
	
#if PLATFORM_MAC || PLATFORM_WINDOWS
	if (SteamAPI_Init())
	{
		for (int32 i = 0; i < Leaderboard.Num(); i++) {
			
			int64 Integer = FCString::Atoi64(*Leaderboard[i].PlayerID->ToString());
			
			CSteamID FriendID = CSteamID((uint64)Integer);
			int Picture = SteamFriends()->GetMediumFriendAvatar(FriendID);

			ArrayTextures.Add(ConvertIntToTexture(Picture));
		}
	//	return ConvertIntToTexture(Picture);
	}
#endif
	return ArrayTextures;
}

UTexture2D* UNetGameInstance::SteamFriendAvatar(uint64 IDSteam) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
	if (SteamAPI_Init())
	{
		//Getting the PictureID from the SteamAPI and getting the Size with the ID
		CSteamID FriendID = CSteamID(IDSteam);
		int Picture = SteamFriends()->GetMediumFriendAvatar(FriendID);

		return ConvertIntToTexture(Picture);
	}
#endif
	return nullptr;
}

UTexture2D* UNetGameInstance::SteamAchievementIcon(FString AchievementName) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
	if (SteamAPI_Init())
	{
		int Picture = SteamUserStats()->GetAchievementIcon(TCHAR_TO_UTF8(*AchievementName));

		return ConvertIntToTexture(Picture);
	}
#endif
	return nullptr;
}


UTexture2D* UNetGameInstance::ConvertIntToTexture(int Picture) {
	uint32 Width;
	uint32 Height;
#if PLATFORM_MAC || PLATFORM_WINDOWS
	SteamUtils()->GetImageSize(Picture, &Width, &Height);

	if (Width > 0 && Height > 0)
	{
		//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
		//BYTE *oAvatarRGBA = new BYTE[Width * Height * 4];
		uint8 *oAvatarRGBA = new uint8[Width * Height * 4];

		//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
		SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));
		UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
		
		//MAGIC!
		uint8* MipData = (uint8*)Avatar->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
		Avatar->PlatformData->Mips[0].BulkData.Unlock();


		//Setting some Parameters for the Texture and finally returning it
		Avatar->PlatformData->NumSlices = 1;
		Avatar->NeverStream = true;
		//Avatar->Rename(TEXT("Avatar"));
		Avatar->CompressionSettings = TC_EditorIcon;

		Avatar->UpdateResource();

		return Avatar;
	}
#endif
	return nullptr;
}


void UNetGameInstance::CheckControllers_Implementation(){
}

//ReplaySystem

void UNetGameInstance::StartRecReplay(){
    TArray <FString> AdditionalOptions;
    FDateTime RecTime = FDateTime::Now();
    StartRecordingReplay("HIVE_"+RecTime.ToString(), "HIVE_"+RecTime.ToString(), AdditionalOptions);
}
void UNetGameInstance::StopRecReplay(){
    StopRecordingReplay();
}
void UNetGameInstance::PlayRecReplay(FString RecName){
    TArray <FString> AdditionalOptions;
    UWorld* world = GetWorld();
    if(world){
        PlayReplay(RecName, world, AdditionalOptions);
    }
}

void UNetGameInstance::FindReplays()
{
    if (EnumerateStreamsPtr.Get())
    {

        EnumerateStreamsPtr.Get()->EnumerateStreams(FNetworkReplayVersion(), FString(), FString(), OnEnumerateStreamsCompleteDelegate);
    }
}

void UNetGameInstance::OnEnumerateStreamsComplete(const TArray<FNetworkReplayStreamInfo>& StreamInfos)
{
    TArray<FS_ReplayInfo> AllReplays;

    for (FNetworkReplayStreamInfo StreamInfo : StreamInfos)
    {
        if (!StreamInfo.bIsLive)
        {
            AllReplays.Add(FS_ReplayInfo(StreamInfo.Name, StreamInfo.FriendlyName, StreamInfo.Timestamp, StreamInfo.LengthInMS));
        }
    }
    
    BP_OnFindReplaysComplete(AllReplays);
}

void UNetGameInstance::PlayReplay(const FString& InName, UWorld* WorldOverride, const TArray<FString>& AdditionalOptions)
{
    Super::PlayReplay(InName, WorldOverride, AdditionalOptions);
    _HandlePreLoadMap = FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UNetGameInstance::HandlePreLoadMap);
}
void UNetGameInstance::HandlePreLoadMap(const FString& MapName)
{
    FCoreUObjectDelegates::PreLoadMap.Remove(_HandlePreLoadMap);
    GetWorld()->DemoNetDriver = nullptr; // The solution lies in this line
}


//UTexture2D * UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(const FBPUniqueNetId UniqueNetId, EBlueprintAsyncResultSwitch &Result, SteamAvatarSize AvatarSize)
//{
//#if PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX
//    if (!UniqueNetId.IsValid() || !UniqueNetId.UniqueNetId->IsValid())
//    {
//        UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("GetSteamFriendAvatar Had a bad UniqueNetId!"));
//        Result = EBlueprintAsyncResultSwitch::OnFailure;
//        return nullptr;
//    }
//    
//    uint32 Width = 0;
//    uint32 Height = 0;
//    
//    if (SteamAPI_Init())
//    {
//        //Getting the PictureID from the SteamAPI and getting the Size with the ID
//        //virtual bool RequestUserInformation( CSteamID steamIDUser, bool bRequireNameOnly ) = 0;
//        
//        
//        uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());
//        int Picture = 0;
//        
//        switch(AvatarSize)
//        {
//            case SteamAvatarSize::SteamAvatar_Small: Picture = SteamFriends()->GetSmallFriendAvatar(id); break;
//            case SteamAvatarSize::SteamAvatar_Medium: Picture = SteamFriends()->GetMediumFriendAvatar(id); break;
//            case SteamAvatarSize::SteamAvatar_Large: Picture = SteamFriends()->GetLargeFriendAvatar(id); break;
//            default: break;
//        }
//        
//        if (Picture == -1)
//        {
//            Result = EBlueprintAsyncResultSwitch::AsyncLoading;
//            return NULL;
//        }
//        
//        SteamUtils()->GetImageSize(Picture, &Width, &Height);
//        
//        // STOLEN FROM ANSWERHUB :p, then fixed because answerhub wasn't releasing the memory O.o
//        // Also fixed image pixel format and switched to a memcpy instead of manual iteration.
//        // At some point I should probably reply to that answerhub post with these fixes to prevent people killing their games.....
//        
//        if (Width > 0 && Height > 0)
//        {
//            //Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
//            uint8 *oAvatarRGBA = new uint8[Width * Height * 4];
//            
//            
//            //Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
//            SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));
//            
//            
//            // Removed as I changed the image bit code to be RGB, I think the original author was unaware that there were different pixel formats
//            /*
//             //Swap R and B channels because for some reason the games whack
//             for (uint32 i = 0; i < (Width * Height * 4); i += 4)
//             {
//             uint8 Temp = oAvatarRGBA[i + 0];
//             oAvatarRGBA[i + 0] = oAvatarRGBA[i + 2];
//             oAvatarRGBA[i + 2] = Temp;
//             }*/
//            
//            UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
//            
//            // Switched to a Memcpy instead of byte by byte transer
//            uint8* MipData = (uint8*)Avatar->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
//            FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
//            Avatar->PlatformData->Mips[0].BulkData.Unlock();
//            
//            // Original implementation was missing this!!
//            // the hell man......
//            delete[] oAvatarRGBA;
//            
//            //Setting some Parameters for the Texture and finally returning it
//            Avatar->PlatformData->NumSlices = 1;
//            Avatar->NeverStream = true;
//            //Avatar->CompressionSettings = TC_EditorIcon;
//            
//            Avatar->UpdateResource();
//            
//            Result = EBlueprintAsyncResultSwitch::OnSuccess;
//            return Avatar;
//        }
//        else
//        {
//            UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("Bad Height / Width with steam avatar!"));
//        }
//        
//        Result = EBlueprintAsyncResultSwitch::OnFailure;
//        return nullptr;
//    }
//#endif
//    
//    UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("STEAM Couldn't be verified as initialized"));
//    Result = EBlueprintAsyncResultSwitch::OnFailure;
//    return nullptr;
//}

void UNetGameInstance::InternetConnectionChecker(){

	AGM_MainMenu* MenuGameMode = Cast<AGM_MainMenu>(GetWorld()->GetAuthGameMode());
	if (!MenuGameMode) {
		return;
	}

	(new FAutoDeleteAsyncTask<FInternetChecker>(ConnectedToInternet))->StartBackgroundTask();

	if(ConnectedToInternet){
	GetTimerManager().SetTimer(ConnectionHandle, this, &UNetGameInstance::InternetConnectionChecker, 5.f, false);
	    //GetOuter()->GetWorld()->GetTimerManager().SetTimer(ConnectionHandle, this, &UNetGameInstance::InternetConnectionChecker, 5.f, false);
	} else {
	    GetTimerManager().SetTimer(ConnectionHandle, this, &UNetGameInstance::InternetConnectionChecker, 1.f, false);
	}




	//AGM_MainMenu* MenuGameMode = Cast<AGM_MainMenu>(GetWorld()->GetAuthGameMode());
 //   if (!MenuGameMode){
 //       return;
 //   }
	//
	////if (!InternetSocket) {
	//	InternetSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	////}

 //   FString address = TEXT("172.217.24.110");
 //   int32 port = 80;
 //   FIPv4Address ip;
 //   
 //   FIPv4Address::Parse(address, ip);
 //   
 //   TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
 //   addr->SetIp(ip.Value);
 //   addr->SetPort(port);

	//if (InternetSocket) {
	//	ConnectedToInternet = InternetSocket->Connect(*addr);
	//}

 //   if(ConnectedToInternet){
	//	GEngine->AddOnScreenDebugMessage(3, 10.f, FColor::Green, TEXT("CONECTED"));
	//	GetTimerManager().SetTimer(ConnectionHandle, this, &UNetGameInstance::InternetConnectionChecker, 5.f, false);
 //       //GetOuter()->GetWorld()->GetTimerManager().SetTimer(ConnectionHandle, this, &UNetGameInstance::InternetConnectionChecker, 5.f, false);
 //   } else {
 //       GEngine->AddOnScreenDebugMessage(3, 10.f, FColor::Red, TEXT("DISCONECTED"));
 //       GetTimerManager().SetTimer(ConnectionHandle, this, &UNetGameInstance::InternetConnectionChecker, 1.f, false);
 //   }
}


void UNetGameInstance::CacheAchievements(class APlayerController* PlayerController)
{
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
	{
		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();

		if (SubsystemInstance)
		{
			
			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
			if (Identity.IsValid())
			{
				
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
				if (UserId.IsValid())
				{
					IOnlineAchievementsPtr AchievementsInterface = SubsystemInstance->GetAchievementsInterface();
					if (AchievementsInterface.IsValid())
					{
						AchievementsInterface->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnCacheAchievementsComplete));
					}
				}
			}
		}
	}
}


void UNetGameInstance::OnCacheAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GetCachedAchievements();
	}
	
}


void UNetGameInstance::GetCachedAchievements()
{
	CachedAchievements.Empty();

	ULocalPlayer* LocalPlayer = GetPrimaryPlayerController()->GetLocalPlayer();

	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
	{

		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();

		if (SubsystemInstance)
		{
			/** Make sure we got the player **/
			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
			if (Identity.IsValid())
			{
				/** Make sure the player controller actually exists **/
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
				if (UserId.IsValid())
				{
					IOnlineAchievementsPtr AchievementsInterface = SubsystemInstance->GetAchievementsInterface();
					if (AchievementsInterface.IsValid())
					{
						CachedAchievementsEarly.SetNumUninitialized(0);
						AchievementsInterface->GetCachedAchievements(*UserId.Get(), CachedAchievementsEarly);

						for (int32 i = 0; i < CachedAchievementsEarly.Num(); i++) {
							FLocalOnlineAchievement Achievement;
							Achievement.Id = CachedAchievementsEarly[i].Id;
							Achievement.Progress = CachedAchievementsEarly[i].Progress;

							FOnlineAchievementDesc AchievementDescription;
							if (AchievementsInterface->GetCachedAchievementDescription(Achievement.Id, AchievementDescription) == EOnlineCachedResult::Success)
							{
								Achievement.bFoundID = true;
								Achievement.Title = AchievementDescription.Title;
								Achievement.LockedDescription = AchievementDescription.LockedDesc;
								Achievement.UnlockedDescription = AchievementDescription.UnlockedDesc;
								Achievement.bHidden = AchievementDescription.bIsHidden;
								//@TODO: UnlockTime - FDateTime is not exposed to Blueprints right now, see TTP 315420
							}

							CachedAchievements.Add(Achievement);
						}
					}
				}
			}
		}
	}
	
}




void UNetGameInstance::WriteAchievements(TArray<FName> AchievementsKeys, TArray<float> AchievementsValues)
{
	ULocalPlayer* LocalPlayer = GetPrimaryPlayerController()->GetLocalPlayer();
	
	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
	{
		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();

		if (SubsystemInstance)
		{
			/** Make sure we got the player **/
			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
			if (Identity.IsValid())
			{
				/** Make sure the player controller actually exists **/
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
				if (UserId.IsValid())
				{
					IOnlineAchievementsPtr AchievementsInterface = SubsystemInstance->GetAchievementsInterface();

					if (AchievementsInterface.IsValid())
					{
						FOnlineAchievementsWritePtr AchievementObject = MakeShareable(new FOnlineAchievementsWrite());
						//TArray<FName> AchievementsStats;
						//Achievements.GetKeys(AchievementsStats);
						for (int32 i = 0; i < AchievementsKeys.Num(); i++) {
							AchievementObject->SetFloatStat(AchievementsKeys[i], AchievementsValues[i]);
						}
						FOnlineAchievementsWriteRef AchievementObjectRef = AchievementObject.ToSharedRef();
						AchievementsInterface->WriteAchievements(*UserId.Get(), AchievementObjectRef, FOnAchievementsWrittenDelegate::CreateUObject(this, &UNetGameInstance::OnWriteAchievementsComplete));

						//AchievementsInterface->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &USteamStatsInfo::OnQueryAchievementsComplete));
					}
				}
			}
		}
	}
}

void UNetGameInstance::OnWriteAchievementsComplete(const FUniqueNetId& PlayerId ,const bool bWasSuccessful){
	if (bWasSuccessful)
	{
	}
	else 
	{
	}
}


void UNetGameInstance::ResetStats()
{
	ULocalPlayer* LocalPlayer = GetPrimaryPlayerController()->GetLocalPlayer();

	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
	{
		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();

		if (SubsystemInstance)
		{
			/** Make sure we got the player **/
			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
			if (Identity.IsValid())
			{
				/** Make sure the player controller actually exists **/
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
				if (UserId.IsValid())
				{

					IOnlineAchievementsPtr AchievementsInterface = SubsystemInstance->GetAchievementsInterface();

					if (AchievementsInterface.IsValid())
					{
//						AchievementsInterface->ResetStats(true);
					}
				}
			}
		}
	}
}

void UNetGameInstance::ReadLeaderboards(FName NameOfLeaderboard) {

	//ULocalPlayer* LocalPlayer = GetPrimaryPlayerController()->GetLocalPlayer();
	//TSharedPtr<const FUniqueNetId> UserId = GetPrimaryPlayerController()->PlayerState->UniqueId.GetUniqueNetId();
	//
	//if (UserId.IsValid() && LocalPlayer && LocalPlayer->GetControllerId() != -1)
	//{
	//	IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
	//	if (SubsystemInstance)
	//	{
	//		/** Make sure we got the player **/
	//		IOnlineLeaderboardsPtr LeaderboardsInterface = SubsystemInstance->GetLeaderboardsInterface();

	//		if (LeaderboardsInterface.IsValid())
	//		{
	//		/*	LeaderboardReadCompleteDelegate = FOnLeaderboardReadCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnReadCompleteLeaderboard);
	//			LeaderboardReadCompleteDelegateHandle = LeaderboardsInterface->AddOnLeaderboardReadCompleteDelegate_Handle(LeaderboardReadCompleteDelegate);*/

	//			if (LeaderboardsInterface.IsValid())
	//			{
	//				//FOnlineLeaderboardReadRef LeaderboardReadObjectRef;

	//				//LeaderboardReadObjectRef = MakeShareable(new FOnlineLeaderboardRead()); //= Leaderboard(new FOnlineLeaderboardRead());
	//				//LeaderboardReadObjectRef->LeaderboardName = NameOfLeaderboard;
	//				//LeaderboardReadObjectRef->SortedColumn = "0";
	//				//LeaderboardReadObjectRef->ReadState = EOnlineAsyncTaskState::NotStarted;
	//				//
	//				//
	//				//LeaderboardReadObject = LeaderboardReadObjectRef.Get();
	//				////LeaderboardReadObjectRef = LeaderboardReadObject.ToSharedRef();


	//				//TArray< TSharedRef<const FUniqueNetId> > ListOfIDs;
	//				//ListOfIDs.Add(UserId.ToSharedRef());
	//				//LeaderboardsInterface->ReadLeaderboards(ListOfIDs, LeaderboardReadObjectRef);


	//				//TSharedPtr<class FOnlineLeaderboardRead, ESPMode::ThreadSafe> ReadObject = MakeShareable(new FOnlineLeaderboardRead());
	//				FOnlineLeaderboardReadRef ReadObject(new FOnlineLeaderboardRead());
	//				ReadObject->LeaderboardName = NameOfLeaderboard;
	//				//ReadObject->SortedColumn = "0";
	//				//new (ReadObject->ColumnMetadata) FColumnMetaData(StatName, StatType);

	//				// Register the completion callback
	//				LeaderboardReadCompleteDelegate = FOnLeaderboardReadCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnReadCompleteLeaderboard);
	//				LeaderboardReadCompleteDelegateHandle = LeaderboardsInterface->AddOnLeaderboardReadCompleteDelegate_Handle(LeaderboardReadCompleteDelegate);

	//				TArray< TSharedRef<const FUniqueNetId> > ListOfIDs;
	//				ListOfIDs.Add(UserId.ToSharedRef());

	//				//FOnlineLeaderboardReadRef ReadObjectRef = ReadObject.Get();
	//				LeaderboardReadObject = &ReadObject.Get();
	//				LeaderboardsInterface->ReadLeaderboards(ListOfIDs, ReadObject);
	//			}
	//			
	//		}
	//	}
	//}

	
}

void UNetGameInstance::OnReadCompleteLeaderboard(bool bIsSuccess) {
	//
	//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, FString::Printf(TEXT("OnReadCompleteLeaderboard")));

	////LeaderboardReadeableData.SetNumUninitialized(0);
	//TArray<FName> ColumnsData;
	//for (int32 i = 0; i < LeaderboardReadObject->Rows.Num(); i++) {
	//	
	//	//TArray<FVariantData> Keys;
	//	//LeaderboardReadObject->Rows[i].Columns.GenerateValueArray(Keys);
	//	//for (int32 j = 0; j < Keys.Num(); j++) {
	//	//	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, FString::Printf(TEXT("OnReadCompleteLeaderboard loop: %s"),  *Keys[j].ToString()));
	//	//}
	//	//FLeaderboardRowConcept Row;
	//	//Row.UserName = LeaderboardReadObject->Rows[i].NickName;
	//	//TArray<FVariantData> Keys;
	//	//LeaderboardReadObject->Rows[i].Columns.GenerateValueArray(Keys);
	//	//int32 Value;
	//	//Keys[0].GetValue(Value);
	//	//Row.StatValue = Value;
	//	//Row.Rank = LeaderboardReadObject->Rows[i].Rank;
	//	//Row.PlayerID = LeaderboardReadObject->Rows[i].PlayerId->ToString();
	//	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, FString::Printf(TEXT("OnReadCompleteLeaderboard: %s - %d - %d"), *LeaderboardReadObject->Rows[i].NickName, LeaderboardReadObject->Rows[i].Rank, LeaderboardReadObject->Rows[i].Columns.Num()));

	//	//LeaderboardReadeableData.Add(Row);
	//}


}


void UNetGameInstance::GetAllUserValuesOfLeaderboard(FName NameOfLeaderboard)
{
	ULocalPlayer* LocalPlayer = GetPrimaryPlayerController()->GetLocalPlayer();

	//if (LeaderboardReadObject) {
	//	LeaderboardReadObject->Rows.Empty();
	//}

	//if (LeaderboardReadeableData.Num() > 0) {
	//	LeaderboardReadeableData.Empty();
	//}


	FOnlineLeaderboardReadRef Leaderboard(new FOnlineLeaderboardRead());
	Leaderboard->LeaderboardName = NameOfLeaderboard;
	Leaderboard->SortedColumn = "0";
	Leaderboard->ReadState = EOnlineAsyncTaskState::NotStarted;
	FColumnMetaData ColumnMetadata("ELO", EOnlineKeyValuePairDataType::Int32);
	Leaderboard->ColumnMetadata.Add(ColumnMetadata);
	
	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
	{
		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
		if (SubsystemInstance)
		{
			/** Make sure we got the player **/
			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
			
			if (Identity.IsValid())
			{
				if (true)
				{

					IOnlineLeaderboardsPtr LeaderboardsInterface = SubsystemInstance->GetLeaderboardsInterface();
					//LeaderboardsInterface->LeaderboardCallback.BindUObject(this, &UNetGameInstance::OnCompleteLeaderboard);

					LeaderboardReadCompleteDelegate = FOnLeaderboardReadCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnCompleteLeaderboard);
					LeaderboardReadCompleteDelegateHandle = LeaderboardsInterface->AddOnLeaderboardReadCompleteDelegate_Handle(LeaderboardReadCompleteDelegate);


					if (LeaderboardsInterface.IsValid())
					{
						LeaderboardReadObject = &Leaderboard.Get();
						
						LeaderboardsInterface->ReadLeaderboardsAroundRank(1, 10, Leaderboard);
					}
				}
			}
		}
	}
}

void UNetGameInstance::OnCompleteLeaderboard(bool bIsSuccess/*FOnlineLeaderboardReadRef Leaderboard*/) {
	UE_LOG(LogShooter, Warning, TEXT("OnCompleteLeaderboard"));

	if (LeaderboardReadObject->Rows.Num() <= 0) { return; }

	TArray<TSharedRef<const FUniqueNetId>>  UserNetIds;

	LeaderboardReadeableData.SetNumUninitialized(0);
	TArray<FName> ColumnsData;
	for (int32 i = 0; i < LeaderboardReadObject->Rows.Num(); i++) {
		UE_LOG(LogShooter, Warning, TEXT("OnCompleteLeaderboard USERNAME %s, LEADER %s, RANK %d"), *LeaderboardReadObject->Rows[i].NickName, *LeaderboardReadObject->LeaderboardName.ToString(), LeaderboardReadObject->Rows[i].Rank);
		FLeaderboardRowConcept Row;
		Row.UserName = LeaderboardReadObject->Rows[i].NickName;
		Row.PlayerID = LeaderboardReadObject->Rows[i].PlayerId.ToSharedRef();

		if (LeaderboardReadObject->Rows[i].PlayerId.IsValid()) {
			UserNetIds.Add(LeaderboardReadObject->Rows[i].PlayerId.ToSharedRef());
		}
		
		TArray<FVariantData> Keys;
		LeaderboardReadObject->Rows[i].Columns.GenerateValueArray(Keys);
		int32 Value;
		Keys[0].GetValue(Value);
		Row.StatValue = Value;
		Row.Rank = LeaderboardReadObject->Rows[i].Rank;
		LeaderboardReadeableData.Add(Row);
	}
	
	int32 Index = GetPrimaryPlayerController()->GetLocalPlayer()->GetControllerId();

	IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();
	if (SubsystemInstance)
	{
		IOnlineUserPtr UserInterface = SubsystemInstance->GetUserInterface();

		if (UserInterface.IsValid())
		{
			bool QuerySuccess = false;
			FString ErrorString;
			FOnQueryUserInfoCompleteDelegate QueryUserInfoCompleteDelegate = FOnQueryUserInfoCompleteDelegate::CreateUObject(this, &UNetGameInstance::OnCompleteQueryUserInfo);
			LeaderboardReadCompleteDelegateHandle = UserInterface->AddOnQueryUserInfoCompleteDelegate_Handle(Index, QueryUserInfoCompleteDelegate);
			UserInterface->QueryUserInfo(Index, UserNetIds);
		}
	}
}

void UNetGameInstance::OnCompleteQueryUserInfo(int32 UserIndex, bool QuerySuccess, const TArray< TSharedRef<const FUniqueNetId> >& UserNetIds, const FString& ErrorString) {

	
	int32 Index = GetPrimaryPlayerController()->GetLocalPlayer()->GetControllerId();

	IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();

	IOnlineUserPtr UserInterface = SubsystemInstance->GetUserInterface();
	
	FString DisplayName;
	TArray<FName> ColumnsData;
	for (int32 i = 0; i < LeaderboardReadeableData.Num(); i++) {
		for (int32 j = 0; j < UserNetIds.Num(); j++) {
			if (LeaderboardReadeableData[i].PlayerID == UserNetIds[j]) {
				FString Atribbute = FString(TEXT("image"));
				FString URL = FString(TEXT(""));
				UserInterface->GetUserInfo(0, UserNetIds[i].Get())->GetUserAttribute(Atribbute, URL);
				FString UserName = UserInterface->GetUserInfo(0, UserNetIds[i].Get())->GetDisplayName(DisplayName);
				LeaderboardReadeableData[i].UserName = UserName;
				LeaderboardReadeableData[i].AvatarUrl = URL;
				UE_LOG(LogShooter, Warning, TEXT("OnCompleteQueryUserInfo NAME: %s - URL: %s"), *UserInterface->GetUserInfo(Index, UserNetIds[i].Get())->GetDisplayName(DisplayName), *URL);
			}
		}
	}
}


void UNetGameInstance::WriteLeaderboardStat(FName StatName, float Value)
{
	ULocalPlayer* LocalPlayer = GetPrimaryPlayerController()->GetLocalPlayer();

	FOnlineLeaderboardWrite WriteObject;
	WriteObject.LeaderboardNames.Add(StatName);
	WriteObject.RatedStat = StatName;
	WriteObject.DisplayFormat = ELeaderboardFormat::Number;
	WriteObject.SortMethod = ELeaderboardSort::Descending;
	WriteObject.UpdateMethod = ELeaderboardUpdateMethod::Force;
	//WriteObject.SetFloatStat(StatName, Value);
	WriteObject.SetIntStat(StatName, (int32)Value);

	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
	{
		IOnlineSubsystem* SubsystemInstance = IOnlineSubsystem::Get();

		if (SubsystemInstance)
		{
			/** Make sure we got the player **/
			IOnlineIdentityPtr Identity = SubsystemInstance->GetIdentityInterface();
			if (Identity.IsValid())
			{
				/** Make sure the player controller actually exists **/
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
				if (UserId.IsValid())
				{
					IOnlineLeaderboardsPtr LeaderboardsInterface = SubsystemInstance->GetLeaderboardsInterface();
					if (LeaderboardsInterface.IsValid())
					{
						LeaderboardsInterface->WriteLeaderboards(GameSessionName, *UserId, WriteObject);
						//LeaderboardsInterface->FlushLeaderboards(SessionName);
					}
				}
			}
		}
	}
}


