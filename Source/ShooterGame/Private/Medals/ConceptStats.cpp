// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "M_Spree.h"
#include "M_MassiveSpree.h"
#include "M_Killer.h"
#include "M_God.h"
#include "M_Inmortal.h"
#include "M_Superman.h"
#include "M_Flash.h"
#include "M_RangerTexas.h"
#include "M_Calcuta.h"
#include "M_Friendzone.h"
#include "M_BePositive.h"
#include "M_YouRocks.h"
#include "M_FriendlyDeath.h"
#include "M_FriendlyFire.h"
#include "M_AfterDeath.h"
#include "M_Nemesis.h"
#include "M_Vendetta.h"
#include "M_Redemption.h"
#include "M_Masochistic.h"
#include "M_Granma.h"
#include "M_CareFree.h"
#include "M_FinalKill.h"
#include "M_Careful.h"
#include "M_Helper.h"
#include "M_BestFriend.h"
#include "M_LetMeOne.h"
#include "M_NoFear.h"
#include "ConceptUserCloudStats.h"
#include "ConceptStats.h"


AConceptStats::AConceptStats()
{
	bAlwaysRelevant = true;

	ActiveMedals.SetNumUninitialized(0);

	ActiveMedals.Add(NewObject<UM_Spree>(UM_Spree::StaticClass()));
	ActiveMedals.Add(NewObject<UM_MassiveSpree>(UM_MassiveSpree::StaticClass()));
	ActiveMedals.Add(NewObject<UM_Killer>(UM_Killer::StaticClass()));
	ActiveMedals.Add(NewObject<UM_God>(UM_God::StaticClass()));
	ActiveMedals.Add(NewObject<UM_Inmortal>(UM_Inmortal::StaticClass()));
	ActiveMedals.Add(NewObject<UM_Flash>(UM_Flash::StaticClass()));
	ActiveMedals.Add(NewObject<UM_Superman>(UM_Superman::StaticClass()));
	ActiveMedals.Add(NewObject<UM_RangerTexas>(UM_RangerTexas::StaticClass()));
	ActiveMedals.Add(NewObject<UM_Calcuta>(UM_Calcuta::StaticClass()));
	ActiveMedals.Add(NewObject<UM_Friendzone>(UM_Friendzone::StaticClass())); //WORKS
	ActiveMedals.Add(NewObject<UM_BePositive>(UM_BePositive::StaticClass()));
	ActiveMedals.Add(NewObject<UM_YouRocks>(UM_YouRocks::StaticClass()));	//WORKS
	ActiveMedals.Add(NewObject<UM_FriendlyDeath>(UM_FriendlyDeath::StaticClass()));
	ActiveMedals.Add(NewObject<UM_FriendlyFire>(UM_FriendlyFire::StaticClass()));
	ActiveMedals.Add(NewObject<UM_AfterDeath>(UM_AfterDeath::StaticClass()));
	ActiveMedals.Add(NewObject<UM_Vendetta>(UM_Vendetta::StaticClass()));	//WORKS
	ActiveMedals.Add(NewObject<UM_Nemesis>(UM_Nemesis::StaticClass()));	//WORKS
	ActiveMedals.Add(NewObject<UM_Redemption>(UM_Redemption::StaticClass()));	//WORKS		
	ActiveMedals.Add(NewObject<UM_Masochistic>(UM_Masochistic::StaticClass()));	//WORKS
	ActiveMedals.Add(NewObject<UM_CareFree>(UM_CareFree::StaticClass()));		//WORKS
	ActiveMedals.Add(NewObject<UM_FinalKill>(UM_FinalKill::StaticClass()));	//WORKS
	ActiveMedals.Add(NewObject<UM_Helper>(UM_Helper::StaticClass()));	
	ActiveMedals.Add(NewObject<UM_BestFriend>(UM_BestFriend::StaticClass()));	
	ActiveMedals.Add(NewObject<UM_LetMeOne>(UM_LetMeOne::StaticClass()));	
	ActiveMedals.Add(NewObject<UM_NoFear>(UM_NoFear::StaticClass()));
	

	bReplicates = true;
	SetReplicates(true);

		KillStrike = 0;

		Assist = 0;

		FirstKill = false;

		FinalKill = false;

		Killers.SetNumUninitialized(0);

		Damage = 0;

		Experience = 0;

		Death = false;

		Nemesis = false;

		AfterDeath = false;

		MVP = false;

		MyNemesis = NULL;

		NemesisVendeta = false;

		NemesisDying = false;

		FriendlyFire = false;

		FriendlyDeath = false;
		
		FirstDeath = false;

		TimeSprinting = 0;

		TimeFlying = 0;

		TimeCrouched = 0;

		KillYourself = 0;

		Deaths = 0;

		HightestRatio = false;

		KillLastEnemyWhoKilledYou = 0;

		Victims.SetNumUninitialized(0);
}

void AConceptStats::InitStats(AConceptPlayerState* MyPlayerState){
	ParentPlayerState = MyPlayerState;

	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	UserCloudStats = GameInstance->UserCloudStats;

	if (UserCloudStats) {
		UserCloudStats->PushToConceptStats(this);
	}

	//InitialELO = MyPlayerState->GetStatValue(ESteamStats::RE_ELO);
	//ELO = InitialELO;
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("InitStats INITIALELO: %d"), InitialELO));
	//if (InitialELO < 100) {
	//	InitialELO = 1500;
	//	ELO = InitialELO;
	//	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("InitStats ELO: %d"), ELO));
	//}

	//DelayedInitStats();
	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptStats::DelayedInitStats);
	GetWorldTimerManager().SetTimer(Handle, Delegate, 5.0f, false);
	
}

void AConceptStats::DelayedInitStats() {
	//InitialELO = ParentPlayerState->GetStatValue(ESteamStats::RE_ELO);
	ELO = InitialELO;
	if (InitialELO < 100) {
		InitialELO = 1500;
		ELO = 1500;
	}

	ParentPlayerState->SetELOReplicated_Server(InitialELO);
}


void AConceptStats::SyncronizeStatsWithSteam(){
	if (!UserCloudStats) {return;}

	//SyncronizeIncrementableStats();
	//SyncronizeOverridableStats();
	FString UserCloudStatsName = FString(TEXT("usercloudstats"));

	UserCloudStats->GetFromConceptStats(this);

	AConceptPlayerController* LocalPlayerController = ParentPlayerState->MyPlayerController;

	if (!LocalPlayerController || !LocalPlayerController->IsPrimaryPlayer()) { return; }

	UGameplayStatics::SaveGameToSlot(UserCloudStats, UserCloudStatsName, 0);

	
	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(LocalPlayerController->GetGameInstance());
	GameInstance->SaveFileToUserCloud(UserCloudStatsName, LocalPlayerController);
	
	
	GameInstance->WriteLeaderboardStat("ELO", ELO);

	TMap<FName, float> Achievements;

#if PLATFORM_WINDOWS || PLATFORM_MAC
	////GOLD
	//Achievements.Add("MLG", UserCloudStats->ELO >= 2000 ? 100.0f : 0.0);
	//Achievements.Add("GOD", UserCloudStats->Kills >= 10000 ? 100.0f : 0.0);
	//Achievements.Add("BOSS", UserCloudStats->NumMatchesW >= 100 ? 100.0f : 0.0);
	//Achievements.Add("NO LIFE", UserCloudStats->TimeGameplay >= 100000 ? 100.0f : 0.0);
	//Achievements.Add("DUDE...", UserCloudStats->Assist >= 10000 ? 100.0f : 0.0);
	//Achievements.Add("CARELESS", UserCloudStats->Deaths >= 10000 ? 100.0f : 0.0);
	//Achievements.Add("UNSTOPPABLE", UserCloudStats->KillStrike >= 30 ? 100.0f : 0.0);

	////SILVER
	//Achievements.Add("PRO GAMER", UserCloudStats->ELO >= 1800 ? 100.0f : 0.0);
	//Achievements.Add("BORN TO WIN", UserCloudStats->NumMatchesW >= 25 ? 100.0f : 0.0);
	//Achievements.Add("THE CONQUERORS", UserCloudStats->HillsConquered >= 1000 ? 100.0f : 0.0);
	//Achievements.Add("SERIAL KILLER", UserCloudStats->Kills >= 1000 ? 100.0f : 0.0);
	//Achievements.Add("SKYDIVER", UserCloudStats->TimeFlying >= 10000 ? 100.0f : 0.0);
	//Achievements.Add("GETTING LATE", UserCloudStats->TimeSprinting >= 10000 ? 100.0f : 0.0);
	//Achievements.Add("HURT", UserCloudStats->DamageReceived >= 10000 ? 100.0f : 0.0);
	//Achievements.Add("FLASH", UserCloudStats->Dashes >= 10000 ? 100.0f : 0.0);
	//Achievements.Add("FRIENDZONE", UserCloudStats->Assist >= 1000 ? 100.0f : 0.0);
	//Achievements.Add("GOOD DAY", UserCloudStats->KillStrike >= 10 ? 100.0f : 0.0);

	////BRONZE
	//Achievements.Add("PRO", UserCloudStats->ELO >= 1600 ? 100.0f : 0.0);
	//Achievements.Add("THE BEGGINING", UserCloudStats->NumMatchesW >= 1 ? 100.0f : 0.0);
	//Achievements.Add("DUDE IM STILL ALIVE", UserCloudStats->Deaths >= 100 ? 100.0f : 0.0);
	//Achievements.Add("LET ME ONE", UserCloudStats->Assist >= 100 ? 100.0f : 0.0);
	//Achievements.Add("TRANSPORT FOR FOUR", UserCloudStats->Teleports >= 100 ? 100.0f : 0.0);
	//Achievements.Add("SORRY", UserCloudStats->Kills >= 100 ? 100.0f : 0.0);
	//Achievements.Add("ROLL OVER THE WORLD", UserCloudStats->Rolls >= 1000 ? 100.0f : 0.0);


	//GOLD
	Achievements.Add("1", UserCloudStats->ELO >= 2000 ? 100.0f : 0.0);
	Achievements.Add("2", UserCloudStats->Kills >= 10000 ? 100.0f : 0.0);
	Achievements.Add("3", UserCloudStats->NumMatchesW >= 100 ? 100.0f : 0.0);
	Achievements.Add("4", UserCloudStats->TimeGameplay >= 100000 ? 100.0f : 0.0);
	Achievements.Add("5", UserCloudStats->Assist >= 10000 ? 100.0f : 0.0);
	Achievements.Add("6", UserCloudStats->Deaths >= 10000 ? 100.0f : 0.0);
	Achievements.Add("7", UserCloudStats->KillStrike >= 30 ? 100.0f : 0.0);

	//SILVER
	Achievements.Add("8", UserCloudStats->ELO >= 1800 ? 100.0f : 0.0);
	Achievements.Add("9", UserCloudStats->NumMatchesW >= 25 ? 100.0f : 0.0);
	Achievements.Add("10", UserCloudStats->HillsConquered >= 1000 ? 100.0f : 0.0);
	Achievements.Add("11", UserCloudStats->Kills >= 1000 ? 100.0f : 0.0);
	Achievements.Add("12", UserCloudStats->TimeFlying >= 10000 ? 100.0f : 0.0);
	Achievements.Add("13", UserCloudStats->TimeSprinting >= 10000 ? 100.0f : 0.0);
	Achievements.Add("14", UserCloudStats->DamageReceived >= 10000 ? 100.0f : 0.0);
	Achievements.Add("15", UserCloudStats->Dashes >= 10000 ? 100.0f : 0.0);
	Achievements.Add("16", UserCloudStats->Assist >= 1000 ? 100.0f : 0.0);
	Achievements.Add("17", UserCloudStats->KillStrike >= 10 ? 100.0f : 0.0);

	//BRONZE
	Achievements.Add("18", UserCloudStats->ELO >= 1600 ? 100.0f : 0.0);
	Achievements.Add("19", UserCloudStats->NumMatchesW >= 1 ? 100.0f : 0.0);
	Achievements.Add("20", UserCloudStats->Deaths >= 100 ? 100.0f : 0.0);
	Achievements.Add("21", UserCloudStats->Assist >= 100 ? 100.0f : 0.0);
	Achievements.Add("22", UserCloudStats->Teleports >= 100 ? 100.0f : 0.0);
	Achievements.Add("23", UserCloudStats->Kills >= 100 ? 100.0f : 0.0);
	Achievements.Add("24", UserCloudStats->Rolls >= 1000 ? 100.0f : 0.0);
#endif

#if PLATFORM_PS4
	//GOLD
	Achievements.Add("1", UserCloudStats->ELO >= 2000 ? 100.0f : 0.0);
	Achievements.Add("2", UserCloudStats->Kills >= 10000 ? 100.0f : 0.0);
	Achievements.Add("3", UserCloudStats->NumMatchesW >= 100 ? 100.0f : 0.0);
	Achievements.Add("4", UserCloudStats->TimeGameplay >= 100000 ? 100.0f : 0.0);
	Achievements.Add("5", UserCloudStats->Assist >= 10000 ? 100.0f : 0.0);
	Achievements.Add("6", UserCloudStats->Deaths >= 10000 ? 100.0f : 0.0);
	Achievements.Add("7", UserCloudStats->KillStrike >= 30 ? 100.0f : 0.0);

	//SILVER
	Achievements.Add("8", UserCloudStats->ELO >= 1800 ? 100.0f : 0.0);
	Achievements.Add("9", UserCloudStats->NumMatchesW >= 25 ? 100.0f : 0.0);
	Achievements.Add("10", UserCloudStats->HillsConquered >= 1000 ? 100.0f : 0.0);
	Achievements.Add("11", UserCloudStats->Kills >= 1000 ? 100.0f : 0.0);
	Achievements.Add("12", UserCloudStats->TimeFlying >= 10000 ? 100.0f : 0.0);
	Achievements.Add("13", UserCloudStats->TimeSprinting >= 10000 ? 100.0f : 0.0);
	Achievements.Add("14", UserCloudStats->DamageReceived >= 10000 ? 100.0f : 0.0);
	Achievements.Add("15", UserCloudStats->Dashes >= 10000 ? 100.0f : 0.0);
	Achievements.Add("16", UserCloudStats->Assist >= 1000 ? 100.0f : 0.0);
	Achievements.Add("17", UserCloudStats->KillStrike >= 10 ? 100.0f : 0.0);

	//BRONZE
	Achievements.Add("18", UserCloudStats->ELO >= 1600 ? 100.0f : 0.0);
	Achievements.Add("19", UserCloudStats->NumMatchesW >= 1 ? 100.0f : 0.0);
	Achievements.Add("20", UserCloudStats->Deaths >= 100 ? 100.0f : 0.0);
	Achievements.Add("21", UserCloudStats->Assist >= 100 ? 100.0f : 0.0);
	Achievements.Add("22", UserCloudStats->Teleports >= 100 ? 100.0f : 0.0);
	Achievements.Add("23", UserCloudStats->Kills >= 100 ? 100.0f : 0.0);
	Achievements.Add("24", UserCloudStats->Rolls >= 1000 ? 100.0f : 0.0);

#endif

	TArray<FName> Keys;
	TArray<float> Values;
	Achievements.GenerateKeyArray(Keys);
	Achievements.GenerateValueArray(Values);

	GameInstance->WriteAchievements(Keys, Values);
}


void AConceptStats::SyncronizeIncrementableStats() {
	TMap<ESteamStats, float> StatsMapIncrements;
	//StatsMapIncrements.Add(ESteamStats::RE_ELO, ELO);
	StatsMapIncrements.Add(ESteamStats::RE_Experience, Experience);
	StatsMapIncrements.Add(ESteamStats::RE_Assists, Assist);
	StatsMapIncrements.Add(ESteamStats::RE_Kills, Kills);
	StatsMapIncrements.Add(ESteamStats::RE_Deaths, Deaths);
	StatsMapIncrements.Add(ESteamStats::RE_Dashes, Dashes);
	StatsMapIncrements.Add(ESteamStats::RE_Jumps, Jumps);
	StatsMapIncrements.Add(ESteamStats::RE_Damage_Received, Damage);
	StatsMapIncrements.Add(ESteamStats::RE_Damage_Done, DamageDone);
	StatsMapIncrements.Add(ESteamStats::RE_Hills_Conquered, HillsConquered);
	StatsMapIncrements.Add(ESteamStats::RE_Num_Matches, NumMatches);
	StatsMapIncrements.Add(ESteamStats::RE_Num_Matches_L, NumMatchesL);
	StatsMapIncrements.Add(ESteamStats::RE_Num_Matches_W, NumMatchesW);
	StatsMapIncrements.Add(ESteamStats::RE_Rolls, Rolls);
	StatsMapIncrements.Add(ESteamStats::RE_Teamwork, Teamwork);
	StatsMapIncrements.Add(ESteamStats::RE_Teleports, Teleports);
	StatsMapIncrements.Add(ESteamStats::RE_Time_Flying, TimeFlying);
	StatsMapIncrements.Add(ESteamStats::RE_Time_Gameplay, TimeGameplay);
	StatsMapIncrements.Add(ESteamStats::RE_Time_Sprinting, TimeSprinting);


	TArray<ESteamStats> Keys;
	StatsMapIncrements.GenerateKeyArray(Keys);
	for (int32 i = 0; i < Keys.Num(); i++) {
		float Value = 0;
		Value = *StatsMapIncrements.Find(Keys[i]);
//		ParentPlayerState->WriteIncrementLocalStat(Keys[i], Value);

		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESteamStats"), true);
		if (!EnumPtr) return;

		FString ToReturn = EnumPtr->GetEnumName((int32)Keys[i]);
	}
}


void AConceptStats::SyncronizeOverridableStats() {
	TMap<ESteamStats, float> StatsMap;
	StatsMap.Add(ESteamStats::RE_ELO, ELO);
	TArray<ESteamStats> Keys;
	StatsMap.GenerateKeyArray(Keys);
	for (int32 i = 0; i < Keys.Num(); i++) {
		float Value = 0;
		Value = *StatsMap.Find(Keys[i]);
//		ParentPlayerState->WriteLocalStat(Keys[i], (float)Value);

		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESteamStats"), true);
		if (!EnumPtr) return;

		FString ToReturn = EnumPtr->GetEnumName((int32)Keys[i]);
	}
}

void AConceptStats::AddMedal(UMedal * Medal) {
	if (Medal != NULL) {
		Medals.Add(Medal);
	}
}

void AConceptStats::CheckMedals_Implementation() {
	

	for(int32 i = 0; i < ActiveMedals.Num(); i++){
		AddMedal(ActiveMedals[i]->GetAchievedMedal(this));
	}

	TArray<UMedal *> AchievedMedals;
	Medals.Append(AchievedMedals);

	ExperienceMultiplier = 0;
	for (int32 i = 0; i < Medals.Num(); i++) {
		ExperienceMultiplier *= Medals[i]->Multiplier;
	}


	/*if (Assist >= 5) {
		UMedal* Medal = NewObject<UMedal>(this, UMedal::StaticClass());
		Medal->MedalName = "HELPER";
		Medal->Multiplier = 1.05;
		Medal->TypeIndex = 1;
		Medal->Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/Medals/Icon/Assists__5__Symbol-01.Assists__5__Symbol-01"), NULL, LOAD_None, NULL);


		if (Assist >= 10) {
			Medal->MedalName = "BEST FRIEND";
			Medal->Multiplier = 1.1;
			Medal->Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/Medals/Icon/Assists__10__Symbol-01.Assists__10__Symbol-01"), NULL, LOAD_None, NULL);

			if (Assist >= 15) {
				Medal->MedalName = "PLEASE, LET ME ONE";
				Medal->Multiplier = 1.15;
				Medal->Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/Medals/Icon/Assists__15__Symbol-01.Assists__15__Symbol-01"), NULL, LOAD_None, NULL);
			}
		}

		Medals.Add(Medal);
	}


	if (NemesisDying) {
		UMedal* Medal = NewObject<UMedal>(this, UMedal::StaticClass());
		Medal->MedalName = "ARCHYENEMY";
		Medal->Multiplier = 1.25;
		Medal->TypeIndex = 0;
		Medal->Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/Medals/Icon/Nemesis__10__Symbol-01.Nemesis__10__Symbol-01"), NULL, LOAD_None, NULL);
		Medals.Add(Medal);
	}

	if (MVP) {
		UMedal* Medal = NewObject<UMedal>(this, UMedal::StaticClass());
		Medal->MedalName = "MVP";
		Medal->TypeIndex = 2;
		Medal->Multiplier = 2;
		Medal->Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/Medals/Icon/MVP_Symbol-01.MVP_Symbol-01"), NULL, LOAD_None, NULL);

		Medals.Add(Medal);
	}


	*/
}



void AConceptStats::AddDash() {
	Dashes++;
}

void AConceptStats::AddJump() {
	Jumps++;
}

void AConceptStats::AddGravityChanges() {
	GravityChanges++;
}

void AConceptStats::AddHillConquered() {
	HillsConquered++;
}

void AConceptStats::AddNumMatches() {
	NumMatches++;
}

void AConceptStats::AddNumMatchesW() {
	NumMatchesW++;
}

void AConceptStats::AddNumMatchesL() {
	NumMatchesL++;
}

void AConceptStats::AddRolls() {
	Rolls++;
}

void AConceptStats::AddTeamwork(float Points) {
	Teamwork += Points;
}

void AConceptStats::AddTeleport() {
	Teleports++;
}

void AConceptStats::AddTimeGameplay(float DeltaTime) {
	TimeGameplay += DeltaTime;
}

void AConceptStats::OnKillScored(){
	Kills++;
	KillStrike++;
	UE_LOG(LogShooter, Warning, TEXT("OnKillScored %d"), KillStrike);
}

void AConceptStats::OnDeathScored() {
	Deaths++;
	KillStrike = 0;
	UE_LOG(LogShooter, Warning, TEXT("OnKillScored %d"), KillStrike);
}

void AConceptStats::OnAssistScored(){
	Assist++;
}

void AConceptStats::AddTimeSprinting(float DeltaTime){
	TimeSprinting += DeltaTime;
}

void AConceptStats::AddTimeFlying(float DeltaTime) {
	TimeFlying += DeltaTime;
}

void AConceptStats::AddTimeWalking(float DeltaTime) {
	TimeWalking += DeltaTime;
}

void AConceptStats::FriendlyFireDone() {
	FriendlyFire = true;
}

void AConceptStats::AddDamageReceived(float DamageReceived){
	Damage += DamageReceived;
}

void AConceptStats::AddDamageDone(float DamageDoneParam) {
	DamageDone += DamageDoneParam;
}

void AConceptStats::CheckMostAssists_Implementation(const TArray<AConceptPlayerState*>& PlayerStates) {
	int32 LocalMostAssists = 0;
	AConceptPlayerState* MostAssistsPlayerState = NULL;
	AConceptPlayerState* MainPlayerState =	Cast<AConceptPlayerState>(GetOwner());

	for(int32 i = 0; i < PlayerStates.Num(); i++){
		if(PlayerStates[i]->StatsDataBase && PlayerStates[i]->StatsDataBase->Assist > LocalMostAssists){
			MostAssistsPlayerState = PlayerStates[i];
			LocalMostAssists = PlayerStates[i]->StatsDataBase->Assist;
		}
	}

	if(MostAssistsPlayerState == MainPlayerState){
		MostAssists = true;
	}
}

bool AConceptStats::CheckMostAssists_Validate(const TArray<AConceptPlayerState*>& PlayerStates) {
	return true;
}

void AConceptStats::CheckHightestRatio_Implementation(const TArray<AConceptPlayerState*>& PlayerStates){
	int32 LocalHightestRatio = -10;
	AConceptPlayerState* HightestRatioPlayerState = NULL;
	AConceptPlayerState* MainPlayerState = Cast<AConceptPlayerState>(GetOwner());

	for (int32 i = 0; i < PlayerStates.Num(); i++) {

		float Ratio = 0;
		if(PlayerStates[i]->StatsDataBase && PlayerStates[i]->StatsDataBase->Kills == 0){
			Ratio = PlayerStates[i]->StatsDataBase->Deaths * -1;
			UE_LOG(LogShooter, Warning, TEXT("Kills == 0 %d"), Ratio);
		}
		else if(PlayerStates[i]->StatsDataBase && PlayerStates[i]->StatsDataBase->Deaths == 0){
			Ratio = PlayerStates[i]->StatsDataBase->Kills;
			UE_LOG(LogShooter, Warning, TEXT("Deaths == 0 %d"), Ratio);
		}
		else if (PlayerStates[i]->StatsDataBase){
			Ratio = PlayerStates[i]->StatsDataBase->Kills / PlayerStates[i]->StatsDataBase->Deaths;
			UE_LOG(LogShooter, Warning, TEXT("Kills / %d"), Ratio);
		}

		if (Ratio != 0 && Ratio > LocalHightestRatio) {
			HightestRatioPlayerState = PlayerStates[i];
			LocalHightestRatio = Ratio;
		}
	}

	if (HightestRatioPlayerState == MainPlayerState) {
		HightestRatio = true;
	}
}


void AConceptStats::AddTeamWorkInTeamDeathMatch() {
	if (PlayersTeam0 + PlayersTeam1 == 1) { return; }

	int32 VictoryPoints = NumMatchesW == 1 ? 1 : 0;

	float ADPStage = (((Kills + Assist) / FMath::Clamp(((float)Deaths), 1.0f, 1000.0f)) * 0.4);
	float VictoryStage = (VictoryPoints * 0.2);

	float DamageStage = 0;
	if (Damage == 0) { DamageStage = Damage * 0.001; }
	else { DamageStage = (FMath::Clamp((DamageDone / Damage), 0.0f, 10.0f) * 0.1); }
	//float HillStage = (HillsConquered * 0.025);

	Teamwork = ADPStage + VictoryStage + DamageStage;
	Experience = (Teamwork * 1000 /** ExperienceMultiplier*/);
}

void AConceptStats::AddTeamWorkInKOTH() {
	if (PlayersTeam0 + PlayersTeam1 == 1) { return; }

	int32 VictoryPoints = NumMatchesW == 1 ? 1 : 0;

	float ADPStage = (((Kills + Assist) / FMath::Clamp(((float)Deaths), 1.0f, 1000.0f)) * 0.05);
	float VictoryStage = (VictoryPoints * 0.5);

	float DamageStage = 0;
	if (Damage == 0) { DamageStage = Damage * 0.0005; }
	else{ DamageStage = (FMath::Clamp((DamageDone / Damage), 0.0f, 10.0f) * 0.05); }
	float HillStage = (HillsConquered * 0.025);

	Teamwork = ADPStage + VictoryStage + DamageStage + HillStage;
	Experience = (Teamwork * 1000 /** ExperienceMultiplier*/);


}

void AConceptStats::AddLeaguePoints() {
	if (PlayersTeam0 + PlayersTeam1 == 1) { 
		ELO = InitialELO + 3;
		return; 
	}

	int32 NumPlayers = 0;
	float ELOEnemies = 0;
	int32 K = 32;
	for (TActorIterator<AConceptPlayerState> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->Team != ParentPlayerState->Team) {
			NumPlayers++;
			ELOEnemies += ActorItr->StatsDataBase->InitialELO;
		}
	}
	
	ELOEnemies = ELOEnemies / NumPlayers;
	
	float ExpectedPoints = 1.0f / (1.0f + FMath::Pow(10, ((ELOEnemies - InitialELO) / 400)));
	
	ELO = (int32)(InitialELO + K*(Teamwork - (ExpectedPoints/ TeamBalance)));

}

void AConceptStats::AddPlayersByTeam(int32 Team0, int32 Team1) {
	PlayersTeam0 = Team0;
	PlayersTeam1 = Team1;

	if (ParentPlayerState->Team == 0 && PlayersTeam1 >0) {
		TeamBalance = 2 - PlayersTeam0 / PlayersTeam1;
	}

	else if(ParentPlayerState->Team == 1 && PlayersTeam0 > 0){
		TeamBalance = 2 - PlayersTeam1 / PlayersTeam0;
	}

	else {
		TeamBalance = 1;
	}

}

void AConceptStats::OnKillPlayerStateComparation(AConceptPlayerState* VictimPlayerState){
	AConceptPlayerState* KillerPlayerState = Cast<AConceptPlayerState>(GetOwner());
	Victims.Add(VictimPlayerState);

	if (KillerPlayerState->Team == VictimPlayerState->Team) {
		
		FriendlyDeath = true;
		FriendlyFire = true;
	}

	if (Killers.Num() > 0 && Killers.HeapTop() == VictimPlayerState) {
		KillLastEnemyWhoKilledYou++;
	}

	//if (Victims.Num() >= 5 && Victims[Victims.Num() - 1] == Victims[Victims.Num() - 2] && Victims[Victims.Num() - 2] == Victims[Victims.Num() - 3] && Victims[Victims.Num() - 3] == Victims[Victims.Num() - 4] && Victims[Victims.Num() - 4] == Victims[Victims.Num() - 5])
	//{
	//	Nemesis = true;
	//}

	int32 NemesisDeaths = 0;
	for (int32 i = 0; i < Victims.Num(); i++)
	{
		if (Victims[i] == VictimPlayerState) {
			NemesisDeaths++;
		}
	}

	if (NemesisDeaths >= 3) {
		Nemesis = true;
	}


	if (MyNemesis == VictimPlayerState) {
		NemesisVendeta = true;
	}

	if (KillerPlayerState->State != EStateEnum::RE_PLAYING) {
		AfterDeath = true;
	}

}

void AConceptStats::OnDeathPlayerStateComparation(AConceptPlayerState* KillerPlayerState){
	AConceptPlayerState* VictimPlayerState = Cast<AConceptPlayerState>(GetOwner());
	Killers.Add(KillerPlayerState);

	if (KillerPlayerState == VictimPlayerState) {
		KillYourself++;
	}

	//if (Killers.Num() >= 5 && Killers[Killers.Num() - 1] == Killers[Killers.Num() - 2] && Killers[Killers.Num() - 2] == Killers[Killers.Num() - 3] && Killers[Killers.Num() - 3] == Killers[Killers.Num() - 4] && Killers[Killers.Num() - 4] == Killers[Killers.Num() - 5])
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("MyNemesis")));
	//	MyNemesis = KillerPlayerState;
	//}

	int32 NemesisKills = 0;
	for (int32 i = 0; i < Killers.Num(); i++)
	{
		if(Killers[i] == KillerPlayerState){
			NemesisKills++;
		}
	}

	if(NemesisKills >= 3){
		MyNemesis = KillerPlayerState;
	}

	
}


void AConceptStats::SetFirstKill(){
	FirstKill = true;
}

void AConceptStats::SetFirstDeath(){
	FirstDeath = true;
}


void AConceptStats::SetFinalKill(AConceptPlayerState* KillerPlayerState){
	if(GetOwner() == KillerPlayerState){
		FinalKill = true;
	}
}


void AConceptStats::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AConceptStats, Kills);
	DOREPLIFETIME(AConceptStats, Deaths);
	DOREPLIFETIME(AConceptStats, Assist);
	DOREPLIFETIME(AConceptStats, InitialELO);
	
}


bool AConceptStats::CheckHightestRatio_Validate(const TArray<AConceptPlayerState*>& PlayerStates) {return true;}

bool AConceptStats::CheckMedals_Validate() {return true;}