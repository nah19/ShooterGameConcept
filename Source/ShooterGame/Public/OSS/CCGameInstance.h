#pragma once
 
 #include "Engine/GameInstance.h"
 #include "OnlineSessionInterface.h"
 #include "CCGameInstance.generated.h"
 
 UCLASS()
 class SHOOTERGAME_API UCCGameInstance : public UGameInstance
 {
     GENERATED_BODY()
 
     virtual void Init() override;
 
	 void OnSessionInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult);
 
     void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
 };