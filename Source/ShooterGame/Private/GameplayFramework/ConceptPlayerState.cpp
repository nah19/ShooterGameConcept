// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptPlayerState.h"
#include "ConceptCharacter.h"
#include "A_MissileLauncher.h"
#include "ConceptGameMode.h"
#include "LobbyPlayerState.h"
#include "OnlineAchievementsInterface.h"
#include "UnrealNetwork.h"
#include "ConceptUserCloudStats.h"
#include "OSS/NetGameInstance.h"

AConceptPlayerState::AConceptPlayerState() {

	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bReplicateMovement = true;
	NetUpdateFrequency = 20;
	SetReplicates(true);
	LastSpawnPoint = NULL;
	State = EStateEnum::RE_INITIAL_SPECTATOR;

	Resources.Add(1000);
	Resources.Add(1000);
	Resources.Add(1000);

	Experience = 0;
	Lvl = 0;
	Kills = 0;
	Deaths = 0;
	Assist = 0;
	PreviousKills = 0;
	TimeToReappear = 10;

	TimeDeath = TimeToReappear;
}

void AConceptPlayerState::PostInitializeComponents() {
	Super::PostInitializeComponents();
	InitState();
	InitGameAttributes();
}

void AConceptPlayerState::BeginPlay() {
	Super::BeginPlay();
	InitState();
	InitGameAttributes();
}

void AConceptPlayerState::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
//	InitSteamStats();
	InitStatsDataBase();
	

	StatsAddTimeGameplay(DeltaSeconds);

	if (TimeDeath <= 0) { return; }
	TimeDeath -= DeltaSeconds;
	if (TimeDeath < 0) { TimeDeath = 0; }
}

void AConceptPlayerState::InitState() {
	InitOwnState();
	InitStrangerState();
	InitGameAttributes();
}

void AConceptPlayerState::InitOwnState() {
	if (!MyPlayerController) { MyPlayerController = Cast<AConceptPlayerController>(GetOwner()); }
	if (!MyCharacter && MyPlayerController) { MyCharacter = Cast<AConceptCharacter>(MyPlayerController->GetCharacter()); }
	if (!MyGameMode) { MyGameMode = Cast<AConceptGameMode>(GetWorld()->GetAuthGameMode()); }
	if (!MyGameState) { MyGameState = Cast<AConceptGameState>(GetWorld()->GetGameState()); }
}

void AConceptPlayerState::InitStrangerState() {
	if (MyPlayerController) { MyPlayerController->SetMyPlayerState(this); }
	if (MyCharacter) { MyCharacter->SetMyPlayerState(this); }
}

void AConceptPlayerState::InitGameAttributes() {
	LocalSessionName = SessionName;
	//InitSteamStats_Server();
	//InitStatsDataBase_Server();
}

void AConceptPlayerState::SetMyCharacter_Implementation(AConceptCharacter* Char) { MyCharacter = Char; }
void AConceptPlayerState::SetMyPlayerController_Implementation(AConceptPlayerController* PController) { MyPlayerController = PController; }
void AConceptPlayerState::SetMyGameMode_Implementation(AConceptGameMode* Game) { MyGameMode = Game; }
void AConceptPlayerState::SetMyGameState_Implementation(AConceptGameState* State) { MyGameState = State; }
bool AConceptPlayerState::SetMyCharacter_Validate(AConceptCharacter* Char) { return true; }
bool AConceptPlayerState::SetMyPlayerController_Validate(AConceptPlayerController* PController) { return true; }
bool AConceptPlayerState::SetMyGameMode_Validate(AConceptGameMode* Char) { return true; }
bool AConceptPlayerState::SetMyGameState_Validate(AConceptGameState* Char) { return true; }

void AConceptPlayerState::FillVars_Implementation() {
	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	int32 Index = GameInstance->GetIndexOfPlayerData(this);
	if (Index != -1) {
		ClassToSpawn = GameInstance->PlayersData[Index].ClassToSpawn;
		Team = GameInstance->PlayersData[Index].Team;
		AllPlayersData = GameInstance->PlayersData;
		FillVariablesWithGameInstanceData(ClassToSpawn, Team, AllPlayersData);
	}
	else
	{
		MyGameMode->DelayedLogin(MyPlayerController);
	}
	

}

void AConceptPlayerState::FillVariablesWithGameInstanceData_Implementation(UClass* MyClass, int32 MyTeam, const TArray<FPlayerPersistentData>& PlayersData) {
	ClassToSpawn = MyClass;
	Team = MyTeam;
	AllPlayersData = PlayersData;
}

void AConceptPlayerState::DelayedFillVars_Implementation(UClass* MyClass, int32 MyTeam, FPlayerPersistentData PlayerData) {
	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	if (GameInstance) {
		GameInstance->PlayersData.Add(PlayerData);
		const TArray<FPlayerPersistentData>& PlayersData = GameInstance->PlayersData;

		FillVariablesWithGameInstanceData(MyClass, MyTeam, PlayersData);
	}
}


void AConceptPlayerState::SetDataOnGameInstanceFromSaved_Implementation() {
	UConceptUserCloudStats* CuerrentGameSaved = Cast<UConceptUserCloudStats>(UGameplayStatics::LoadGameFromSlot("UserCloudStats", 0));
	if (CuerrentGameSaved != NULL) {
		SetPlayerDataGameInstance(CuerrentGameSaved->DefaultCharacter, CuerrentGameSaved->DefaultAbilities[0], CuerrentGameSaved->DefaultAbilities[1], CuerrentGameSaved->DefaultAbilities[2]);
	}
}


void AConceptPlayerState::SetPlayerDataGameInstance_Implementation(UClass* DefaultCharacter, UClass* DefaultAbility1, UClass* DefaultAbility2, UClass* DefaultAbility3) {
	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	int32 Index = GameInstance->GetIndexOfPlayerData(this);
	FPlayerPersistentData Data;

	if (Index == -1) {
		Data.AbilitiesToSpawn[0] = DefaultAbility1;
		Data.AbilitiesToSpawn[1] = DefaultAbility2;
		Data.AbilitiesToSpawn[2] = DefaultAbility3;
		Data.ClassToSpawn = DefaultCharacter;
		Data.Gametag = this->PlayerName;
		Data.Ready = true;
		int32 LocalTeam = -1;
		if (MyGameMode) {
			LocalTeam = MyGameMode->TeamSelector();
			Data.Team = LocalTeam;
		}
		GameInstance->PlayersData.Add(Data);
		FillVariablesWithGameInstanceData(DefaultCharacter, LocalTeam, GameInstance->PlayersData);
	}
	else {
		FillVariablesWithGameInstanceData(GameInstance->PlayersData[Index].ClassToSpawn, GameInstance->PlayersData[Index].Team, GameInstance->PlayersData);
	}
}

void AConceptPlayerState::SetSavedGameDefaults_Implementation(int32 TeamSelector) {
	PlayerDefaults = Cast<UConceptUserCloudStats>(UGameplayStatics::LoadGameFromSlot("UserCloudStats", 0));

	SetServerSavedGameDefaults(TeamSelector);
}

void AConceptPlayerState::SetServerSavedGameDefaults_Implementation(int32 TeamSelector) {
	FPlayerPersistentData MyPlayerData = FPlayerPersistentData::FPlayerPersistentData();

	MyPlayerData.UniqueId = PlayerId;
	MyPlayerData.ClassToSpawn = PlayerDefaults->DefaultCharacter;
	MyPlayerData.Gametag = PlayerName;
	MyPlayerData.Team = TeamSelector;
	MyPlayerData.AbilitiesToSpawn[0] = PlayerDefaults->DefaultAbilities[0];
	MyPlayerData.AbilitiesToSpawn[1] = PlayerDefaults->DefaultAbilities[1];
	MyPlayerData.AbilitiesToSpawn[2] = PlayerDefaults->DefaultAbilities[2];

	DelayedFillVars(PlayerDefaults->DefaultCharacter, TeamSelector, MyPlayerData);
}


void AConceptPlayerState::KillEventToHUD(FString KillerGameTag, FString VictimGameTag, FString KillerNameAbility, int32 KillerTeam, int32 VictimTeam) {
	KillEventToHUD_Multicast(KillerGameTag, VictimGameTag, KillerNameAbility,KillerTeam, VictimTeam);
}

void AConceptPlayerState::KillEventToHUD_Multicast_Implementation(const FString& KillerGameTag, const FString& VictimGameTag, const FString& KillerNameAbility, int32 KillerTeam, int32 VictimTeam) {
	if (MyPlayerController && MyGameState) {
		MyPlayerController->KillEventToHUD(KillerGameTag, VictimGameTag, KillerNameAbility, KillerTeam, VictimTeam);
	}
}

bool AConceptPlayerState::KillEventToHUD_Multicast_Validate(const FString& KillerGameTag, const FString& VictimGameTag, const FString& KillerNameAbility, int32 KillerTeam, int32 VictimTeam) {
	return true;
}

void AConceptPlayerState::SetTimeDeathMulticast_Implementation(float Time){
	TimeDeath = Time;
}

void AConceptPlayerState::OnDeathStateManager_Implementation() {
	SetTimeDeathMulticast(TimeToReappear);
	DeathCharacter();
	ScoreDeath();
	SetPlayerState(EStateEnum::RE_KILLED);
	
	FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &AConceptPlayerState::OnDeathSpectatorStateManager);
	GetWorldTimerManager().SetTimer(DeathHandle, delegate, 2.5f, false);
}



void AConceptPlayerState::DeathCharacter_Implementation(){
	if(MyPlayerController) {
		MyPlayerController->InputManager(EControllerInput::RE_SPECTATOR);
		if(MyCharacter) {
			MyPlayerController->ClientStopCameraShake(MyCharacter->RunningCameraShake);
			MyCharacter->Death();
		}
	}
	RemoveAbilities();
}

void AConceptPlayerState::OnDeathSpectatorStateManager() {
	
	
	ServerChangeToSpectator();
	MyPlayerController->HUDManager(EHUDStateEnum::RE_DEATHSTATISTICS);
	MyPlayerController->InputManager(EControllerInput::RE_SPECTATOR);

	FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &AConceptPlayerState::OnRespawnStateManager);
	GetWorldTimerManager().SetTimer(RespawnHandle, delegate, TimeToReappear - 2.5f, false);
}

void AConceptPlayerState::ServerChangeToSpectator_Implementation(){
	SetPlayerState(EStateEnum::RE_INITIAL_SPECTATOR);
	AConceptPlayerStart* SpectatorPlayerStart = Cast<AConceptPlayerStart>(MyGameMode->FindPlayerStart((AController*)MyPlayerController));
	bIsSpectator = true;

	MyPlayerController->InputManager(EControllerInput::RE_SPECTATOR);
	MyPlayerController->SetInitialLocationAndRotation(SpectatorPlayerStart->GetActorLocation(), SpectatorPlayerStart->GetActorRotation());
	MyPlayerController->ChangeState(NAME_Spectating);
	MyPlayerController->ClientGotoState(NAME_Spectating);

	ClientChangeToSpectator(SpectatorPlayerStart);
}



void AConceptPlayerState::ChangeToSpectator(AConceptPlayerStart* Start){
	if (Start)
	{
		FVector Location = Start->GetActorLocation();
		FRotator Rotation = Start->GetActorRotation();

		bIsSpectator = true;
		MyPlayerController->SetInitialLocationAndRotation(Location, Rotation);
		MyPlayerController->ChangeState(NAME_Spectating);
		MyPlayerController->ClientGotoState(NAME_Spectating);
	}
}


void AConceptPlayerState::ClientChangeToSpectator_Implementation(AConceptPlayerStart* Start){
	SetPlayerState(EStateEnum::RE_RESPAWNING);
	ChangeToSpectator(Start);
}


void AConceptPlayerState::OnRespawnStateManager() {
	if (HasAuthority()) { RestartPlayerWithState(EStateEnum::RE_PLAYING); }
}

void AConceptPlayerState::ScoreAssist_Implementation() {
	Assist++;

	StatsAssistScored();

//	WriteIncrementStat(ESteamStats::RE_Assists, 1);
}

void AConceptPlayerState::ScoreKill() {
	Kills++;
	
	
	StatsKillScored();
}

void AConceptPlayerState::ScoreDeath() {
	Deaths++;

	StatsDeathScored();
}

void AConceptPlayerState::OnJoinStateManager() {
	MyGameMode = Cast<AConceptGameMode>(GetWorld()->GetAuthGameMode());
	MyGameState = Cast<AConceptGameState>(GetWorld()->GetGameState());

	OnJoinProperMatch_Client();

	if (MyGameMode) {
		if (MyGameMode->GetMatchState() == MatchState::WaitingToStart) {
			MyPlayerController->HUDManager(EHUDStateEnum::RE_STATISTICS);
		}
		else if (MyGameMode->GetMatchState() == MatchState::InProgress) {
			switch (MyGameState->InProgressState) {
			case EInProgressStateEnum::RE_EARLYSTAGE: {
				RestartPlayerWithState(EStateEnum::RE_BLOCKED);
			}
													  break;
			case EInProgressStateEnum::RE_PROPERMATCH: {
				
				RestartPlayerWithState(EStateEnum::RE_PLAYING);
			}
													   break;
			}
		}
	}
}

void AConceptPlayerState::OnJoinProperMatch_Client_Implementation() {
	MyPlayerController = Cast<AConceptPlayerController>(GetOwner());
	if (MyPlayerController) {
		MyPlayerController->ShowInGameHUD();
	}
}


void AConceptPlayerState::SetPlayerState_Implementation(EStateEnum NextState) {
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateEnum"), true);
	if (EnumPtr) UStaticFunctionLibrary::PrintString(this, EnumPtr->GetEnumName((int32)NextState));

	State = NextState;
}

void AConceptPlayerState::SyncronizeStatsWithSteam_Implementation() {
	StatsDataBase->SyncronizeStatsWithSteam();
	//SteamStats->UpdateAllStatsToSteam();
}

void AConceptPlayerState::SyncronizingPlayerState_Implementation(AConceptGameMode* MyGameMode, FName MatchState) {

	AConceptGameState* MyGameState = Cast<AConceptGameState>(GetWorld()->GetGameState());
	if (!MyGameMode) { return; }
	
	if (MyGameMode->GetMatchState() == MatchState::WaitingToStart) {

		MyPlayerController->HUDManager(EHUDStateEnum::RE_PLAYER_LOADER);
		SetPlayerState(EStateEnum::RE_INITIAL_SPECTATOR);
	}

	else if (MyGameMode->GetMatchState() == MatchState::InProgress) {
		SyncronizingPlayerStateInProgress(MyGameState->InProgressState);
	}
	else if (MyGameMode->GetMatchState() == MatchState::WaitingPostMatch) {
		if(MyPlayerController) MyPlayerController->InputManager(EControllerInput::RE_SPECTATOR);
		
		GetWorldTimerManager().ClearAllTimersForObject(this);
		
		int32 Winner = MyGameMode->GetWinnerTeam();

		switch (State) {
			case EStateEnum::RE_KILLED:{
				StatsReplicateDamages_Server();
				DeathCharacter();
				ServerChangeToSpectator();
				MyPlayerController->HUDManager(EHUDStateEnum::RE_FINALSTATISTICS);
				SetPlayerState(EStateEnum::RE_RESULTS);
				UpToSteamMatchesResults(Winner);
				SyncronizeStatsWithSteam();
			}
			break;
			case EStateEnum::RE_PLAYING:{
				StatsReplicateDamages_Server();
				DeathCharacter();
				ServerChangeToSpectator();
				MyPlayerController->HUDManager(EHUDStateEnum::RE_FINALSTATISTICS);
				SetPlayerState(EStateEnum::RE_RESULTS);
				UpToSteamMatchesResults(Winner);
				SyncronizeStatsWithSteam();
			}
			break;
			case EStateEnum::RE_INITIAL_SPECTATOR:{
				StatsReplicateDamages_Server();
				DeathCharacter();
				ServerChangeToSpectator();
				MyPlayerController->HUDManager(EHUDStateEnum::RE_FINALSTATISTICS);
				SetPlayerState(EStateEnum::RE_RESULTS);
				UpToSteamMatchesResults(Winner);
				SyncronizeStatsWithSteam();
			}
			break;
			case EStateEnum::RE_RESPAWNING:{
				StatsReplicateDamages_Server();
				DeathCharacter();
				ServerChangeToSpectator();
				MyPlayerController->HUDManager(EHUDStateEnum::RE_FINALSTATISTICS);
				SetPlayerState(EStateEnum::RE_RESULTS);
				UpToSteamMatchesResults(Winner);
				SyncronizeStatsWithSteam();
			}
			break;
			case EStateEnum::RE_RESULTS:{
				MyPlayerController->HUDManager(EHUDStateEnum::RE_FINALSTATISTICS);
				SetPlayerState(EStateEnum::RE_END_SPECTATOR);
			}
			break;
			case EStateEnum::RE_END_SPECTATOR:{
				MyPlayerController->HUDManager(EHUDStateEnum::RE_NO_HUD);
				SetPlayerState(EStateEnum::RE_END_SPECTATOR);
			}
			break;
			case EStateEnum::RE_LEAVER: {
				SetPlayerState(EStateEnum::RE_LEAVER);
			}
			break;
			default:
				break;
		}
	}
}

void AConceptPlayerState::SyncronizingPlayerStateInProgress(EInProgressStateEnum MatchState) {
	//UE_LOG(LogShooter, Warning, TEXT("SyncronizingPlayerStateInProgress"));

	switch (MatchState) {
		case EInProgressStateEnum::RE_EARLYSTAGE:{
			if (State == EStateEnum::RE_INITIAL_SPECTATOR){
				SetPlayerState(EStateEnum::RE_BLOCKED);
			}
		}
		break;
		case EInProgressStateEnum::RE_PROPERMATCH:{
			if (State == EStateEnum::RE_BLOCKED){
				SetPlayerState(EStateEnum::RE_PLAYING);
			}
		}
		break;
	}
}




void AConceptPlayerState::ServerInitAbilities_Implementation(TSubclassOf<AAbility> Class1, TSubclassOf<AAbility> Class2, TSubclassOf<AAbility> Class3, TSubclassOf<AAbility> Class4, TSubclassOf<AAbility> Class5) {
	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	int32 Index = GameInstance->GetIndexOfPlayerData(this);
	InitState();
	//FUCKED
	if (Index != -1){
		if (CharacterAbilities.Num() == 0){
			Ab1 = GameInstance->PlayersData[Index].AbilitiesToSpawn[0];
			Ab2 = GameInstance->PlayersData[Index].AbilitiesToSpawn[1];
			Ab3 = GameInstance->PlayersData[Index].AbilitiesToSpawn[2];
			Ab4 = Class4;
			Ab5 = Class5;//GameInstance->PlayersData[Index].AbilitiesToSpawn[4];

			CharacterAbilities = InitAbilities(Ab1, Ab2, Ab3, Ab4, Ab5);
			MyCharacter->AbilityInUse = CharacterAbilities[0];
			//MyCharacter->AbilityInUse->StartInUseAbility(0);
		}

	}
	else{

		if (CharacterAbilities.Num() == 0){

			Ab1 = Class1;
			Ab2 = Class2;
			Ab3 = Class3;
			Ab4 = Class4;
			Ab5 = Class5;

			CharacterAbilities = InitAbilities(Class1, Class2, Class3, Class4, Class5);
			MyCharacter->AbilityInUse = CharacterAbilities[0];
			//MyCharacter->AbilityInUse->StartInUseAbility(0);
		}
	}
}

void AConceptPlayerState::AttachAbilities_Implementation(AAbility* AbilityToPass, AConceptCharacter* MyCharacterParam){
	//UE_LOG(LogShooter, Warning, TEXT("AttachAbilities_Implementation"));

	if (AbilityToPass){
		if (MyCharacterParam){
			AbilityToPass->AttachToComponent(MyCharacterParam->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AbilityToPass->CurrentSocketUsed);
		}
	}
}


TArray<AAbility*> AConceptPlayerState::InitAbilities(TSubclassOf<AAbility> Class1, TSubclassOf<AAbility> Class2, TSubclassOf<AAbility> Class3, TSubclassOf<AAbility> Class4, TSubclassOf<AAbility> Class5){

	TArray<AAbility*> AbilitiesToPass;

	FActorSpawnParameters SpawnInfo;

	//TODO: quitar este duplicado
	SpawnInfo.Owner = MyCharacter;

	AAbility* AbilityToPass1 = GetWorld()->SpawnActor<AAbility>(Class1, MyCharacter->GetActorLocation(), MyCharacter->GetActorRotation(), SpawnInfo);
	AbilityToPass1->CurrentSocketUsed = "Weapon0";
	AbilityToPass1->StartInUseAbility();
	AttachAbilities(AbilityToPass1, MyCharacter);


	AAbility* AbilityToPass2 = GetWorld()->SpawnActor<AAbility>(Class2, MyCharacter->GetActorLocation(), MyCharacter->GetActorRotation(), SpawnInfo);
	AbilityToPass2->CurrentSocketUsed = "Weapon1";
	AbilityToPass2->StopInUseAbility();
	AttachAbilities(AbilityToPass2, MyCharacter);


	AAbility* AbilityToPass3 = GetWorld()->SpawnActor<AAbility>(Class3, MyCharacter->GetActorLocation(), MyCharacter->GetActorRotation(), SpawnInfo);
	AbilityToPass3->CurrentSocketUsed = "Weapon2";
	AbilityToPass3->StopInUseAbility();
	AttachAbilities(AbilityToPass3, MyCharacter);


	AAbility* AbilityToPass4 = GetWorld()->SpawnActor<AAbility>(Class4, MyCharacter->GetActorLocation(), MyCharacter->GetActorRotation(), SpawnInfo);
	AbilityToPass4->CurrentSocketUsed = "Weapon3";
	AbilityToPass4->StopInUseAbility();
	AttachAbilities(AbilityToPass4, MyCharacter);



	AAbility* AbilityToPass5 = GetWorld()->SpawnActor<AAbility>(Class5, MyCharacter->GetActorLocation(), MyCharacter->GetActorRotation(), SpawnInfo);
	AbilityToPass5->CurrentSocketUsed = "Weapon4";
	AbilityToPass5->StopInUseAbility();
	AttachAbilities(AbilityToPass5, MyCharacter);
 


	AbilitiesToPass.Add(AbilityToPass1);
	AbilitiesToPass.Add(AbilityToPass2);
	AbilitiesToPass.Add(AbilityToPass3);
	AbilitiesToPass.Add(AbilityToPass4);
	AbilitiesToPass.Add(AbilityToPass5);
	
	return AbilitiesToPass;
}

void AConceptPlayerState::RemoveAbilities_Implementation() {
	for (int32 i = 0; i < CharacterAbilities.Num(); i++) {
		AAbility* Ability = Cast<AAbility>(CharacterAbilities[i]);
		Ability->Destroy();
	}
	CharacterAbilities.Empty();
}

void AConceptPlayerState::UpToSteamMatchesResults_Implementation(int32 Winner) {
	StatsReplicateDamages_Server();

	if (Winner == Team) {
		StatsAddNumMatchesW();
	}
	else if (Winner != -1) {
		StatsAddNumMatchesL();
	}
	StatsAddPlayersTeamBalance();
	StatsAddNumMatches();
	StatsAddTeamwork();
	StatsAddLeaguePoints();
}



void AConceptPlayerState::RestartPlayerWithState_Implementation(EStateEnum StateParam) {

	SetPlayerState(StateParam);
	if (MyGameMode) {
		if (LastSpawnPoint) { 
			MyGameMode->LiberateSpawnPoint(LastSpawnPoint); 
		}

		MyPlayerController = Cast<AConceptPlayerController>(GetOwner());
		if(MyPlayerController){
			MyGameMode->RestartPlayer(MyPlayerController);
		}
		
		if (MyCharacter) { 
			MyCharacter->ReactiveCharacter(); 
		}
	}
}

void AConceptPlayerState::CopyProperties(class APlayerState* PlayerState){
	APlayerState::CopyProperties(PlayerState);
	ALobbyPlayerState* myPlayerState = Cast<ALobbyPlayerState>(PlayerState);

	if (myPlayerState){
		myPlayerState->Team = Team;
	}
}


void AConceptPlayerState::InitStatsDataBase_Server_Implementation() {
	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;

	//StatsDataBase = GetWorld()->SpawnActor<AConceptStats>(AConceptStats::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	if (StatsDataBase) {return;}
	InitStatsDataBase_Multicast();
}
void AConceptPlayerState::InitStatsDataBase_Multicast_Implementation() {
	InitStatsDataBase();
}

void AConceptPlayerState::InitStatsDataBase() {
	if (StatsDataBase) {
		if (!StatsDataBase->ParentPlayerState) {
			StatsDataBase->InitStats(this);
		}
		return; 
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = MyCharacter;
	//SpawnParams.bNoFail = true;
	StatsDataBase = GetWorld()->SpawnActor<AConceptStats>(AConceptStats::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	if (StatsDataBase) {StatsDataBase->InitStats(this);}
	
}

void AConceptPlayerState::SetELOReplicated_Server_Implementation(int32 ELOParam) {
	SetELOReplicated_Multicast(ELOParam);
}

void AConceptPlayerState::SetELOReplicated_Multicast_Implementation(int32 ELOParam) {
	if (StatsDataBase) {
		StatsDataBase->InitialELO = ELOParam;
	}
}

void AConceptPlayerState::DestroyPlayerState() {
	Destroy();
}




void AConceptPlayerState::StatsAddDash() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddDash();
}

void AConceptPlayerState::StatsAddPlayersTeamBalance() {
	if (!StatsDataBase) { return; }

	int32 PlayersTeam0 = 0;
	int32 PlayersTeam1 = 0;

	for (TActorIterator<AConceptPlayerState> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		AConceptPlayerState* PlayerStateItr = *ActorItr;
		if (PlayerStateItr->Team == 0) {
			PlayersTeam0++;
		}
		else {
			PlayersTeam1++;
		}
	}

	StatsDataBase->AddPlayersByTeam(PlayersTeam0, PlayersTeam1);

}

void AConceptPlayerState::StatsAddJump() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddJump();
}

void AConceptPlayerState::StatsAddDamageReceived(float Damage) {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddDamageReceived(Damage);
}

void AConceptPlayerState::StatsAddDamageDone(float Damage) {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddDamageDone(Damage);
}

void AConceptPlayerState::StatsAddGravityChanges() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddGravityChanges();
}

void AConceptPlayerState::StatsAddHillConquered() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddHillConquered();
}

void AConceptPlayerState::StatsAddNumMatches() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddNumMatches();
}

void AConceptPlayerState::StatsAddNumMatchesW() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddNumMatchesW();
}

void AConceptPlayerState::StatsAddNumMatchesL() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddNumMatchesL();
}

void AConceptPlayerState::StatsAddRolls() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddRolls();
}

void AConceptPlayerState::StatsAddTeleport() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddTeleport();
}

void AConceptPlayerState::StatsAddTimeGameplay(float DeltaTime) {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddTimeGameplay(DeltaTime);
}

void AConceptPlayerState::StatsKillScored_Implementation(){
	if (!StatsDataBase) { return; }
	StatsDataBase->OnKillScored();
}
void AConceptPlayerState::StatsDeathScored_Implementation() {
	if (!StatsDataBase) { return; }
	StatsDataBase->OnDeathScored();
}
void AConceptPlayerState::StatsAssistScored_Implementation() {
	if (!StatsDataBase) { return; }
	StatsDataBase->OnAssistScored();
}
void AConceptPlayerState::StatsAddTimeSprinting(float DeltaTime){
	if (!StatsDataBase) { return; }
	StatsDataBase->AddTimeSprinting(DeltaTime);
}
void AConceptPlayerState::StatsAddTimeFlying(float DeltaTime) {
	if (!StatsDataBase) {return;}
	StatsDataBase->AddTimeFlying(DeltaTime);
}

void AConceptPlayerState::StatsAddTimeWalking(float DeltaTime) {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddTimeWalking(DeltaTime);
}

void AConceptPlayerState::StatsFriendlyFireDone_Implementation(){
	if (!StatsDataBase) { return; }
	StatsDataBase->FriendlyFireDone();
}

void AConceptPlayerState::StatsMostAssist_Implementation(const TArray<AConceptPlayerState*>& PlayerStates) {
	if (!StatsDataBase) { return; }
	StatsDataBase->CheckMostAssists(PlayerStates);
}

void AConceptPlayerState::StatsHightestRatio_Implementation(const TArray<AConceptPlayerState*>& PlayerStates){
	if (!StatsDataBase) { return; }
	StatsDataBase->CheckHightestRatio(PlayerStates);
}

void AConceptPlayerState::StatsOnKillPlayerStateComparation(AConceptPlayerState* VictimPlayerState){
	if (!StatsDataBase) { return; }
	StatsDataBase->OnKillPlayerStateComparation(VictimPlayerState);
}

void AConceptPlayerState::StatsOnDeathPlayerStateComparation(AConceptPlayerState* KillerPlayerState){
	if (!StatsDataBase) { return; }
	StatsDataBase->OnDeathPlayerStateComparation(KillerPlayerState);
}

void AConceptPlayerState::StatsFirstKill(){
	if (!StatsDataBase) { return; }
	StatsDataBase->SetFirstKill();
}

void AConceptPlayerState::StatsFirstDeath(){
	if (!StatsDataBase) { return; }
	StatsDataBase->SetFirstDeath();
}

void AConceptPlayerState::StatsFinalKill_Implementation(AConceptPlayerState* KillerPlayerState){
	if (!StatsDataBase) { return; }
	StatsDataBase->SetFinalKill(KillerPlayerState);
}

void AConceptPlayerState::StatsLessDamagePlayer_Server_Implementation(const TArray<AConceptPlayerState*>& PlayerStates){
	TArray<float> Damages;
	for(int32 i = 0; i < PlayerStates.Num(); i++){
		Damages.Add(PlayerStates[i]->StatsDataBase->Damage);
	}

	StatsLessDamagePlayer_Multicast(PlayerStates, Damages);
}

void AConceptPlayerState::StatsLessDamagePlayer_Multicast_Implementation(const TArray<AConceptPlayerState*>& PlayerStates, const TArray<float>& Damages) {
	for (int32 i = 0; i < PlayerStates.Num(); i++) {
		PlayerStates[i]->StatsDataBase->Damage = Damages[i];
	}
}

void AConceptPlayerState::StatsAddTeamwork() { 
	if (!StatsDataBase) { return; }
	StatsDataBase->AddTeamWorkInKOTH(); 
}

void AConceptPlayerState::StatsAddLeaguePoints() {
	if (!StatsDataBase) { return; }
	StatsDataBase->AddLeaguePoints(); 
}

void AConceptPlayerState::StatsReplicateDamages_Server_Implementation() {
	StatsReplicateDamages_Multicast(StatsDataBase->Damage, StatsDataBase->DamageDone);
}

void AConceptPlayerState::StatsReplicateDamages_Multicast_Implementation(float DamageParam, float DamageDoneParam) {
	StatsDataBase->Damage = DamageParam;
	StatsDataBase->DamageDone = DamageDoneParam;
}

void AConceptPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AConceptPlayerState, Race);
	DOREPLIFETIME(AConceptPlayerState, CharacterAbilities);
	DOREPLIFETIME(AConceptPlayerState, State);
	DOREPLIFETIME(AConceptPlayerState, Kills);
	DOREPLIFETIME(AConceptPlayerState, Deaths);
	DOREPLIFETIME(AConceptPlayerState, Assist);
	DOREPLIFETIME(AConceptPlayerState, Lvl);
	DOREPLIFETIME(AConceptPlayerState, MyCharacter);
	DOREPLIFETIME(AConceptPlayerState, MyPlayerController);
	DOREPLIFETIME(AConceptPlayerState, Resources);
	DOREPLIFETIME(AConceptPlayerState, ClassToSpawn);
	DOREPLIFETIME(AConceptPlayerState, Team);
	DOREPLIFETIME(AConceptPlayerState, AllPlayersData);
	DOREPLIFETIME(AConceptPlayerState, StatsDataBase);
	
}

bool AConceptPlayerState::RemoveAbilities_Validate() { return true; }
bool AConceptPlayerState::SyncronizingPlayerState_Validate(AConceptGameMode* MyGameMode, FName MatchState) { return true; }
bool AConceptPlayerState::SyncronizeStatsWithSteam_Validate() { return true; }
bool AConceptPlayerState::OnDeathStateManager_Validate() { return true; }
bool AConceptPlayerState::StatsFriendlyFireDone_Validate() { return true; }
bool AConceptPlayerState::ServerChangeToSpectator_Validate(){ return true; }
bool AConceptPlayerState::ClientChangeToSpectator_Validate(AConceptPlayerStart* Start){ return true; }
bool AConceptPlayerState::SetPlayerState_Validate(EStateEnum NextState) { return true; }
bool AConceptPlayerState::RestartPlayerWithState_Validate(EStateEnum StateParam) { return true; }
bool AConceptPlayerState::ServerInitAbilities_Validate(TSubclassOf<AAbility> Class1, TSubclassOf<AAbility> Class2, TSubclassOf<AAbility> Class3, TSubclassOf<AAbility> Class4, TSubclassOf<AAbility> Class5) { return true; }
bool AConceptPlayerState::SetTimeDeathMulticast_Validate(float Time) {return true;}
bool AConceptPlayerState::AttachAbilities_Validate(AAbility* AbilityToPass, AConceptCharacter* MyCharacter) { return true; }
bool AConceptPlayerState::DeathCharacter_Validate() { return true; }
bool AConceptPlayerState::FillVars_Validate() { return true; }
bool AConceptPlayerState::SetServerSavedGameDefaults_Validate(int32 TeamSelector) { return true; }
bool AConceptPlayerState::SetDataOnGameInstanceFromSaved_Validate() { return true; }
bool AConceptPlayerState::SetPlayerDataGameInstance_Validate(UClass* DefaultCharacter, UClass* DefaultAbility1, UClass* DefaultAbility2, UClass* DefaultAbility3) { return true; }
bool AConceptPlayerState::DelayedFillVars_Validate(UClass* MyClass, int32 MyTeam, FPlayerPersistentData PlayerData) { return true; }
bool AConceptPlayerState::ScoreAssist_Validate() { return true; }
bool AConceptPlayerState::InitStatsDataBase_Multicast_Validate() { return true; }
bool AConceptPlayerState::InitStatsDataBase_Server_Validate() { return true; }
bool AConceptPlayerState::StatsLessDamagePlayer_Multicast_Validate(const TArray<AConceptPlayerState*>& PlayerStates, const TArray<float>& Damages) { return true; }
bool AConceptPlayerState::StatsLessDamagePlayer_Server_Validate(const TArray<AConceptPlayerState*>& PlayerStates) { return true; }
bool AConceptPlayerState::StatsKillScored_Validate() {	return true;}
bool AConceptPlayerState::StatsDeathScored_Validate() {	return true;}
bool AConceptPlayerState::StatsAssistScored_Validate() {	return true;}
bool AConceptPlayerState::StatsMostAssist_Validate(const TArray<AConceptPlayerState*>& PlayerStates) {	return true;}
bool AConceptPlayerState::StatsHightestRatio_Validate(const TArray<AConceptPlayerState*>& PlayerStates) {	return true;}
bool AConceptPlayerState::StatsFinalKill_Validate(AConceptPlayerState* KillerPlayerState) {	return true;}
//bool AConceptPlayerState::StatsAddDamageReceived_Validate(float Damage) {	return true;}
//bool AConceptPlayerState::StatsAddDamageDone_Validate(float Damage) {	return true;}

bool AConceptPlayerState::StatsReplicateDamages_Server_Validate() { return true; }

bool AConceptPlayerState::StatsReplicateDamages_Multicast_Validate(float DamageParam, float DamageDoneParam) {return true;}

bool AConceptPlayerState::SetELOReplicated_Server_Validate(int32 ELOParam) {return true;}

bool AConceptPlayerState::SetELOReplicated_Multicast_Validate(int32 ELOParam) {return true;}

bool AConceptPlayerState::UpToSteamMatchesResults_Validate(int32 Winner) {return true;}
