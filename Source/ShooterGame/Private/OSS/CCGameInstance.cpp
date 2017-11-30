// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "CCGameInstance.h"
#include "OnlineSubsystemUtils.h"


void UCCGameInstance::Init()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = OnlineSub->GetSessionInterface();
		if (SessionInt.IsValid())
		{
			int32 ControllerId = 0;
			if (ControllerId != 255)
			{
				FOnSessionUserInviteAcceptedDelegate OnSessionInviteAcceptedDelegate = FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UCCGameInstance::OnSessionInviteAccepted);
				SessionInt->AddOnSessionUserInviteAcceptedDelegate_Handle(OnSessionInviteAcceptedDelegate);

				FOnJoinSessionCompleteDelegate joinDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UCCGameInstance::OnJoinSessionCompleted);
				SessionInt->AddOnJoinSessionCompleteDelegate_Handle(joinDelegate);
			}
		}
	}
}

void UCCGameInstance::OnSessionInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult)
{
	UE_LOG(LogOnline, Verbose, TEXT("OnSessionInviteAccepted LocalUserNum: %d bSuccess: %d"), LocalUserNum, bWasSuccessful);

	if (bWasSuccessful)
	{
		if (SearchResult.IsValid())
		{
			IOnlineSessionPtr SessionInt = IOnlineSubsystem::Get()->GetSessionInterface();
			SessionInt->JoinSession(LocalUserNum, GameSessionName, SearchResult);
		}
		else
		{
			UE_LOG(LogOnline, Warning, TEXT("Invite accept returned no search result."));
		}
	}
}

void UCCGameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogOnline, Verbose, TEXT("JoinSessionCompleted"));
	IOnlineSessionPtr Sessions = IOnlineSubsystem::Get()->GetSessionInterface();
	if (Sessions.IsValid())
	{
		UE_LOG(LogOnline, Verbose, TEXT("Sessions Valid"));
		if (Result == EOnJoinSessionCompleteResult::Success)
		{
			// Client travel to the server
			FString ConnectString;
			if (Sessions->GetResolvedConnectString(GameSessionName, ConnectString))
			{
				UE_LOG(LogOnline, Log, TEXT("Join session: traveling to %s"), *ConnectString);
				GetFirstLocalPlayerController()->ClientTravel(ConnectString, TRAVEL_Absolute);
			}
		}
	}
}