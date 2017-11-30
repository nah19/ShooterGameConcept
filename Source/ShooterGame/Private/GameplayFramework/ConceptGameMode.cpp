// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "ConceptPlayerController.h"
#include "ConceptGameMode.h"
#include "ConceptCharacter.h"
#include "ConceptPlayerState.h"
#include "ConceptPlayerController.h"
#include "ConceptGameState.h"
#include "ConceptGameSession.h"
#include "ConceptRewardSystem.h"
#include "ConceptSpectatorPawn.h"
#include "StaticFunctionLibrary.h"
#include "NetGameInstance.h"
#include "OSS/NetGameInstance.h"
#include "PlayerSaveGame.h"
#include "PC_C_Lobby.h"


AConceptGameMode::AConceptGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));

	//if (PlayerPawnBPClass.Class != NULL) {
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	PlayerControllerClass = AConceptPlayerController::StaticClass();
	PlayerStateClass = AConceptPlayerState::StaticClass();
	GameStateClass = AConceptGameState::StaticClass();
	SpectatorClass = AConceptSpectatorPawn::StaticClass();

	bUseSeamlessTravel = true;
	bStartPlayersAsSpectators = true;
	bDelayedStart = true;

	MinRespawnDelay = 1;

	CurrentPlayers = 0;
	CurrentPawnType = 0;
	

	MatchState = MatchState::WaitingToStart;
}

void AConceptGameMode::PostInitializeComponents() {
	Super::PostInitializeComponents();
	MyGameState = Cast<AConceptGameState>(GameState);

	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	if (GameInstance) {
		TotalGamePlayers = GameInstance->PlayersData.Num();

		if (GetWorld()->GetNetMode() == NM_ListenServer) {
			GameInstance->UpdateSessionMode(MyGameState->GameModeName);
		}
	}

	if(MyGameState){
		MyGameState->TypeOfGameMode = MyGameState->TypeOfGameMode;
	}
}



APawn* AConceptGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) {
	AConceptPlayerStart* MyStart = Cast<AConceptPlayerStart>(StartSpot);
	
	// don't allow pawn to be spawned with any pitch or roll
	FRotator StartRotation(ForceInit);
	StartRotation = StartSpot->GetActorRotation();
	FVector StartLocation = StartSpot->GetActorLocation();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;

	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	int32 Index = GameInstance->GetIndexOfPlayerData(NewPlayer->PlayerState);

	APawn* ResultPawn = NULL;
	//FUCKED
	if (Index != -1){
		ResultPawn = GetWorld()->SpawnActor<APawn>(GameInstance->PlayersData[Index].ClassToSpawn, StartLocation, StartRotation, SpawnInfo);
	}
	else{
		ResultPawn = GetWorld()->SpawnActor<APawn>(GameInstance->PlayersData[0].ClassToSpawn, StartLocation, StartRotation, SpawnInfo);
	}

	if(MyStart->SpawnTeam == 1 && ResultPawn) {
		AConceptCharacter* CharacterSpawned = Cast<AConceptCharacter>(ResultPawn);
		CharacterSpawned->InitGravity();
		CharacterSpawned->InversePlayer = true;
	}

	return ResultPawn;
}

void AConceptGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	FTimerHandle TimerHandle_DefaultTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AConceptGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void AConceptGameMode::DefaultTimer() {
	if (MyGameState && MyGameState->RemainingTime > 0 && !MyGameState->bTimerPaused) {
		MyGameState->RemainingTime--;

		if (MyGameState->RemainingTime <= 0) {
			if (GetMatchState() == MatchState::WaitingPostMatch && !bWaitingPostMatch) {
				UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - WaitingPostMatch"));
				bWaitingPostMatch = true;
				DestroySession();
				SyncronizingAllPlayerStateCharacters();
			}
			else if (GetMatchState() == MatchState::InProgress) {
				if (MyGameState->InProgressState == EInProgressStateEnum::RE_EARLYSTAGE && !bEarlyStage) {
					UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - RE_EARLYSTAGE"));
					bEarlyStage = true;
					HandleMatchProperMatch();
				}
				else if (MyGameState->InProgressState == EInProgressStateEnum::RE_PROPERMATCH && !bProperMatch) {
					UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - RE_PROPERMATCH"));
					bProperMatch = true;
					FinishingMatch();
				}
			}
			else if (GetMatchState() == MatchState::WaitingToStart && !bWaitingToStart) {
				bWaitingToStart = true;
				UE_LOG(LogShooter, Warning, TEXT("DefaultTimer - bWaitingToStart"));
				StartMatch();
			}
		}
	}
}

void  AConceptGameMode::SyncronizingAllPlayerStateCharacters() {
	for (TActorIterator<AConceptPlayerState> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->MyPlayerController) {
			ActorItr->SyncronizingPlayerState(this, MatchState);
		}
	}
}

void  AConceptGameMode::FillAllPlayerStateWithGameInstanceData() {
	for (TActorIterator<AConceptPlayerState> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		ActorItr->FillVars();
	}
}

void AConceptGameMode::ReactiveAllCharacters() {
	for (TActorIterator<AConceptCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		ActorItr->ReactiveCharacter();
	}
}

void AConceptGameMode::DisableAllCharacters() {
	for (TActorIterator<AConceptCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		ActorItr->GameFinished();
	}
}

void AConceptGameMode::PreLogin(const FString& Options, const FString& Address, const TSharedPtr<const FUniqueNetId>& UniqueId, FString& ErrorMessage) {
	const bool bMatchIsOver = MyGameState && MyGameState->HasMatchEnded();
	if (bMatchIsOver) {
		ErrorMessage = TEXT("Match is over!");
	}
	else {
		// GameSession can be NULL if the match is over
		Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	}
}

void AConceptGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	AConceptPlayerController* NewPC = Cast<AConceptPlayerController>(NewPlayer);
	AConceptPlayerState* MyPlayerState = Cast<AConceptPlayerState>(NewPC->PlayerState);

	CurrentPlayers++;

	if (IsMatchInProgress()) {
		DelayedLogin(NewPC);
		return;
	}

#if WITH_EDITOR
	if (NewPC && NewPC->GetPawn() == NULL) {
		NewPC->ClientSetSpectatorCamera(NewPC->GetSpawnLocation(), NewPC->GetControlRotation());
	}

	int32 Index = TeamSelector() + 1;
	MyPlayerState->SetPlayerName("Prueba" + FString::FromInt(Index));

#endif
}

void AConceptGameMode::DelayedLogin(AConceptPlayerController* NewPlayer) {
	NewPlayer->HUDManager(EHUDStateEnum::RE_PLAYER_LOADER);
	AConceptPlayerState* MyPlayerState = Cast<AConceptPlayerState>(NewPlayer->PlayerState);

	if (!MyPlayerState) return;

//	MyPlayerState->InitSteamStats_Server();
	
	if (NumberOfPlayersT1 >= NumberOfPlayersT2) {
		NumberOfPlayersT2++;
	}
	else {
		NumberOfPlayersT1++;
	}

	MyPlayerState->SetDataOnGameInstanceFromSaved();

	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(MyPlayerState, &AConceptPlayerState::OnJoinStateManager);
	GetWorldTimerManager().SetTimer(Handle, Delegate, MyGameState->WarmupTime, false);
}

void AConceptGameMode::CustomPostLogin_Implementation(APlayerController* NewPlayer) {
	AConceptPlayerController* NewPC = Cast<AConceptPlayerController>(NewPlayer);

	if (NewPC) {
		//if(NewPC->MyPlayerState && NewPC->MyPlayerState->Team == -1){
		//	DelayedLogin(NewPC);
		//	return;
		//}
		CurrentPlayers++;
		if (GetMatchState() == MatchState::WaitingToStart && MyGameState && MyGameState->RemainingTime > 10) {
			if (CurrentPlayers >= TotalGamePlayers) {
				MyGameState->RemainingTime = 10;
			}
		}
		NewPC->ClientSetSpectatorCamera(NewPC->GetSpawnLocation(), NewPC->GetControlRotation());
		
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewport(true);
		InputMode.SetHideCursorDuringCapture(true);

		NewPC->SetInputMode(InputMode);

		if (GetMatchState() == MatchState::EnteringMap || GetMatchState() == MatchState::WaitingToStart) {
			NewPC->HUDManager(EHUDStateEnum::RE_PLAYER_LOADER);
		}
	}
}

APlayerController* AConceptGameMode::SpawnPlayerController(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation) {
	APlayerController* PlayerController = Super::SpawnPlayerController(InRemoteRole, SpawnLocation, SpawnRotation);

	return PlayerController;
}

void AConceptGameMode::UpdatePlayerControllers() {
	AConceptPlayerController* NewPlayerController = GetWorld()->SpawnActor<AConceptPlayerController>(FVector(0, 0, 0), FRotator(0, 0, 0));
	if (NewPlayerController) {
		APlayerController* OldPlayerController = Cast<APlayerController>(GetWorld()->GetFirstLocalPlayerFromController());
		SwapPlayerControllers(OldPlayerController, NewPlayerController);
	}
}

void AConceptGameMode::HandleMatchIsWaitingToStart() {
	UE_LOG(LogShooter, Warning, TEXT("HandleMatchIsWaitingToStart"));
	SyncronizingAllPlayerStateCharacters();
	LiberateAllSpawnPoints();

	if (bDelayedStart) {
		if (MyGameState && MyGameState->RemainingTime == 0) {

			if (MyGameState->WarmupTime > 0) {
				MyGameState->RemainingTime = MyGameState->WarmupTime;
			}
			else {
				MyGameState->RemainingTime = 0;
			}
		}
	}
}

void AConceptGameMode::HandleMatchEarlyStage() {
	UE_LOG(LogShooter, Warning, TEXT("HandleMatchEarlyStage"));
	MyGameState->InProgressState = EInProgressStateEnum::RE_EARLYSTAGE;

	AConceptGameSession* MySession = Cast<AConceptGameSession>(GameSession);
	MySession->StartSession();

	MyGameState->RemainingTime = MyGameState->EarlyStageTime;
	SyncronizingAllPlayerStateCharacters();
}

void AConceptGameMode::HandleMatchProperMatch() {
	UE_LOG(LogShooter, Warning, TEXT("HandleMatchProperMatch"));
	MyGameState->InProgressState = EInProgressStateEnum::RE_PROPERMATCH;

	ReactiveAllCharacters();
	MyGameState->RemainingTime = MyGameState->RoundTime;

	SyncronizingAllPlayerStateCharacters();
}

void AConceptGameMode::HandleMatchHasStarted() {
	UE_LOG(LogShooter, Warning, TEXT("HandleMatchHasStarted"));
	HandleMatchEarlyStage();

	Super::HandleMatchHasStarted();
}

void AConceptGameMode::FinishingMatch(){
	DisableAllCharacters();

	MyGameState->RemainingTime = MyGameState->TimeBetweenMatches;

	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptGameMode::FinishMatch);
	GetWorldTimerManager().SetTimer(Handle, Delegate, 1.5f, false);
}

void AConceptGameMode::FinishMatch() {
	if (IsMatchInProgress()) {
		FillMedals();
		EndMatch();
	}

	SyncronizingAllPlayerStateCharacters();
}


int32 AConceptGameMode::TeamSelector(){
	NumberOfPlayersT1 = 0;
	NumberOfPlayersT2 = 0;
	for (TActorIterator<AConceptPlayerState> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		switch(ActorItr->Team){
		case 0:{
			NumberOfPlayersT1++;
		}
		break;
		case 1: {
			NumberOfPlayersT2++;
		}
		break;
		}
	}

	if (NumberOfPlayersT1 > NumberOfPlayersT2) {
		CurrentPawnType = 1;
		return 1;
	}
	CurrentPawnType = 0;

	return CurrentPawnType;
}


void AConceptGameMode::Killed(AController* Killer, AController* KilledPlayer, FAbilityData AbilityData) {

	//TODO: Si el killer es un minion, no sumar puntos y esas cosas, que peta

	AConceptPlayerState* KillerPlayerState = Killer ? Cast<AConceptPlayerState>(Killer->PlayerState) : NULL;
	AConceptPlayerState* VictimPlayerState = KilledPlayer ? Cast<AConceptPlayerState>(KilledPlayer->PlayerState) : NULL;
	
	FKillData Data;
	
	if (KillerPlayerState && VictimPlayerState){

		Data.KillerNameAbility = AbilityData.NameAbility.ToString();
		Data.KillerTeam = KillerPlayerState->Team;
		Data.VictimTeam = VictimPlayerState->Team;
	
		if (KillerPlayerState == VictimPlayerState) { 
			Data.VictimCharacter = Cast<AConceptCharacter>(KilledPlayer->GetPawn())->NameOfCharacter;
			Data.VictimGameTag = VictimPlayerState->PlayerName;
		}
		else if (KillerPlayerState->Team == VictimPlayerState->Team) {
			Data.KillerCharacter = Cast<AConceptCharacter>(Killer->GetPawn())->NameOfCharacter;
			Data.KillerGameTag = KillerPlayerState->PlayerName;
			Data.VictimCharacter = Cast<AConceptCharacter>(KilledPlayer->GetPawn())->NameOfCharacter;
			Data.VictimGameTag = VictimPlayerState->PlayerName;
		}
		else {
			KillerPlayerState->ScoreKill();
			if (VictimPlayerState->Team == 1) { MyGameState->Team1Statistics[(int32)ETeamStatistics::RE_KILLS]++; }
			else { MyGameState->Team2Statistics[(int32)ETeamStatistics::RE_KILLS]++; }

			UConceptRewardSystem::ManageResourcesOnKill(KillerPlayerState, VictimPlayerState);
			Data.KillerCharacter = Cast<AConceptCharacter>(Killer->GetPawn())->NameOfCharacter;
			Data.KillerGameTag = KillerPlayerState->PlayerName;
			Data.VictimCharacter = Cast<AConceptCharacter>(KilledPlayer->GetPawn())->NameOfCharacter;
			Data.VictimGameTag = VictimPlayerState->PlayerName;
		}
		MyGameState->SetMedalsOnKill(VictimPlayerState, KillerPlayerState);

		

		MyGameState->AddKillInfo(Data);
		DeadEvent(Data);
	}

	if (VictimPlayerState) {
		VictimPlayerState->OnDeathStateManager();
	}

}

void AConceptGameMode::DeadEvent(FKillData Data){

		FString KillerGameTag = Data.KillerGameTag;

		FString KillerCharacter = Data.KillerCharacter;

		FString KillerNameAbility = Data.KillerNameAbility;

		FString VictimGameTag = Data.VictimGameTag;

		FString VictimCharacter= Data.VictimCharacter;

		int32 KillerTeam = Data.KillerTeam;

		int32 VictimTeam = Data.VictimTeam;


	for (TActorIterator<AConceptPlayerState> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			ActorItr->KillEventToHUD(KillerGameTag, VictimGameTag, KillerNameAbility, KillerTeam, VictimTeam);
		}
	}
}

void AConceptGameMode::SortPlayerStartsByPriority(AController* Player, TArray<APlayerStart*> &PreferredSpawns, TArray<APlayerStart*> &FallbackSpawns){
	for (FActorIterator It(GetWorld()); It; ++It) {
		APlayerStart* TestSpawn = Cast<APlayerStart>(*It);
		if (TestSpawn != NULL) {
			AConceptPlayerStart* Point = Cast<AConceptPlayerStart>(TestSpawn);
			if (/*Point && */IsSpawnpointAllowed(TestSpawn, Player)) {
				if (IsSpawnpointPreferred(TestSpawn, Player)) {
					PreferredSpawns.Add(TestSpawn);
				}
				else {
					FallbackSpawns.Add(TestSpawn);
				}
			}
		}
	}
}

AActor* AConceptGameMode::ChoosePlayerStart_Implementation(AController* Player) {
	TActorIterator< APlayerStart > PlayerStartIterator = TActorIterator<APlayerStart>(GetWorld());
	AConceptPlayerStart* ConceptSpawnPoint = Cast<AConceptPlayerStart>(Cast<APlayerStart>(*PlayerStartIterator));
	AConceptPlayerState* ConceptPlayerState = Cast<AConceptPlayerState>(Player->PlayerState);
	TArray<APlayerStart*> PreferredSpawns;
	TArray<APlayerStart*> FallbackSpawns;

	SortPlayerStartsByPriority(Player, PreferredSpawns, FallbackSpawns);


	if (PreferredSpawns.Num() > 0) {
		ConceptSpawnPoint = Cast<AConceptPlayerStart>(PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())]);
		if (ConceptSpawnPoint){
			ConceptSpawnPoint->SetPlayerStateLink(ConceptPlayerState);
		}
	}
	else if (FallbackSpawns.Num() > 0) {
		ConceptSpawnPoint = Cast<AConceptPlayerStart>(FallbackSpawns[FMath::RandHelper(FallbackSpawns.Num())]);
		if (ConceptSpawnPoint){
			ConceptSpawnPoint->SetPlayerStateLink(ConceptPlayerState);
		}
	}
	return ConceptSpawnPoint;
}

void AConceptGameMode::LiberateSpawnPoint(AConceptPlayerStart* SpawnPoint) {
	if (SpawnPoint != NULL){SpawnPoint->RemovePlayerStateLink();}
}

void AConceptGameMode::LiberateAllSpawnPoints() {
	for (FActorIterator It(GetWorld()); It; ++It) {
		AConceptPlayerStart* TestSpawn = Cast<AConceptPlayerStart>(*It);
		if (TestSpawn) {
			TestSpawn->RemovePlayerStateLink();
		}
	}
}

bool AConceptGameMode::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const {
	AConceptPlayerStart* ConceptSpawnPoint = Cast<AConceptPlayerStart>(SpawnPoint);
	AConceptPlayerState* ConceptPlayerState = Cast<AConceptPlayerState>(Player->PlayerState);

	if (ConceptSpawnPoint) {
		if ((ConceptPlayerState->State == EStateEnum::RE_INITIAL_SPECTATOR || ConceptPlayerState->State == EStateEnum::RE_END_SPECTATOR || ConceptPlayerState->State == EStateEnum::RE_RESPAWNING) && ConceptSpawnPoint->PlayerStartTag == "Spectator") {return true;}
		if (ConceptPlayerState->State == EStateEnum::RE_INITIAL_SPECTATOR || ConceptPlayerState->State == EStateEnum::RE_END_SPECTATOR || ConceptPlayerState->State == EStateEnum::RE_RESPAWNING) {return false;}
		if (ConceptSpawnPoint->PlayerStartTag == "Spectator") {return false;}
		if (ConceptSpawnPoint->bNotForPlayers || ConceptSpawnPoint->SpawnTeam != ConceptPlayerState->Team) {return false;}
	}
	return true;
}


bool AConceptGameMode::IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Player) const {
	AConceptPlayerStart* ConceptSpawnPoint = Cast<AConceptPlayerStart>(SpawnPoint);
	if (ConceptSpawnPoint) {
		return !ConceptSpawnPoint->CheckIfBusy();
	}
	return true;
}

void AConceptGameMode::DestroySession_Implementation() {
	UNetGameInstance* Instance = Cast<UNetGameInstance>(GetGameInstance());
	Instance->DestroySessionAndLeaveGame();
	//for (TActorIterator<AConceptPlayerController> ControllerItr(GetWorld()); ControllerItr; ++ControllerItr) {
	//	ControllerItr->OnDestroySession();
	//}
}

//bool AConceptGameMode::HasMatchStarted() const
//{
//	if (GetMatchState() == MatchState::EnteringMap || GetMatchState() == MatchState::WaitingToStart)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool AConceptGameMode::IsMatchInProgress() const
//{
//	if (GetMatchState() == MatchState::InProgress)
//	{
//		return true;
//	}
//
//	return false;
//}

void AConceptGameMode::InactivePlayer(AConceptPlayerController* Controller) {/*Overrided*/ }

void AConceptGameMode::FillMedals() { MyGameState->InitMedals(); }

int32 AConceptGameMode::GetWinnerTeam() { return -1; /*Overrided*/ }

bool AConceptGameMode::ShouldSpawnAtStartSpot(AController* Player) {return false;}

TSubclassOf<AGameSession> AConceptGameMode::GetGameSessionClass() const {return AConceptGameSession::StaticClass();}

AGameSession* AConceptGameMode::GetGameSession(){return GameSession;}

