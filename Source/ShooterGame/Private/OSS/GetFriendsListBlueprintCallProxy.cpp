
#include "ShooterGame.h"
#include "GetFriendsListBlueprintCallProxy.h"


UGetFriendsListBlueprintCallProxy::UGetFriendsListBlueprintCallProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	//This sets the Delegate so we can be notified when the subsystem has completed reading the friends list.
	, ReadCompleteDelegate(FOnReadFriendsListComplete::CreateUObject(this, &ThisClass::OnReadFriendsListComplete))
{
}


UGetFriendsListBlueprintCallProxy* UGetFriendsListBlueprintCallProxy::GetFriendsList(UObject* WorldContextObject, class APlayerController* PlayerController)
{

	UE_LOG(LogShooter, Warning, TEXT("GetFriendsList"));
	UGetFriendsListBlueprintCallProxy* Proxy = NewObject<UGetFriendsListBlueprintCallProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;

	Proxy->WorldContextObject = WorldContextObject;

	UE_LOG(LogShooter, Warning, TEXT("GetFriendsListEnd"));
	return Proxy;
}

void UGetFriendsListBlueprintCallProxy::Activate()
{
	UE_LOG(LogShooter, Warning, TEXT("Activate"));
	//This gets the OnlineSubsystem

	auto Friends = Online::GetFriendsInterface();
	if (Friends.IsValid())
	{
		ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerControllerWeakPtr->Player);
		//Friends->ReadFriendsList AddOnReadFriendsListComplete(Player->ControllerId, ReadCompleteDelegate);
		Friends->ReadFriendsList(Player->GetControllerId(), EFriendsLists::ToString(EFriendsLists::Default), ReadCompleteDelegate);


		return;
	}



	// Fail immediately
	TArray<FBlueprintFriend> Fail;

	OnFailure.Broadcast(Fail);

	UE_LOG(LogShooter, Warning, TEXT("ActivateEnd"));
}

void UGetFriendsListBlueprintCallProxy::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	UE_LOG(LogShooter, Warning, TEXT("OnReadFriendsListComplete"));
	if (bWasSuccessful)
	{
		auto Friends = Online::GetFriendsInterface();
		if (Friends.IsValid())
		{
			TArray<FBlueprintFriend>FriendsArr;
			ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerControllerWeakPtr->Player);
			TArray< TSharedRef<FOnlineFriend> > OutFriends;

			Friends->GetFriendsList(LocalUserNum, ListName, OutFriends);
			for (int32 i = 0; i < OutFriends.Num(); i++)
			{
				TSharedRef<FOnlineFriend> Friend = OutFriends[i];
				//This creates a BlueprintFriend, a custom class for blueprints, because I could not get anything other to work
				FBlueprintFriend BPFriend = FBlueprintFriend();

				BPFriend.Presence = EOnlinePresenceState::ToString(Friend->GetPresence().Status.State);
				BPFriend.DisplayName = Friend->GetDisplayName();
				BPFriend.RealName = Friend->GetRealName();
				BPFriend.UniqueNetId = Friend->GetUserId()->ToString();
				FriendsArr.Add(BPFriend);


			}
			//Friends->ClearOnReadFriendsListCompleteDelegate(LocalUserNum, ReadCompleteDelegate);
			OnSuccess.Broadcast(FriendsArr);



		}
	}
	else
	{
		TArray<FBlueprintFriend> Fail;


		OnFailure.Broadcast(Fail);
	}

	UE_LOG(LogShooter, Warning, TEXT("OnReadFriendsListCompleteEnd"));
}