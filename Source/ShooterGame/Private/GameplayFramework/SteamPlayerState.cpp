// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "SteamPlayerState.h"

ASteamPlayerState::ASteamPlayerState() {

	
}

void ASteamPlayerState::BeginPlay() {
	Super::BeginPlay();

	//SetPlayerName(GetCustomPlayerName());

	SteamPlayerController = Cast<APlayerController>(GetOwner());

	//InitSteamStats_Server();
}

void ASteamPlayerState::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	//InitUserCloudStats();
}

//void ASteamPlayerState::InitSteamStats()
//{
//	APlayerController* LocalPlayerController = Cast<APlayerController>(GetOwner());
//
//	if(!SteamStats){ 
//		SteamStats = NewObject<USteamStatsInfo>(this, USteamStatsInfo::StaticClass()); 
//		if (LocalPlayerController && LocalPlayerController->GetLocalPlayer()) {
//			SteamStats->InitStats(LocalPlayerController->GetLocalPlayer(), SessionName);
//			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("InitSteamStats")));
//		}
//		return;
//	}
//	else if (!SteamStats->LocalPlayer && LocalPlayerController && LocalPlayerController->GetLocalPlayer()) {
//		SteamStats->InitStats(LocalPlayerController->GetLocalPlayer(), SessionName);
//	}
//}

void ASteamPlayerState::InitUserCloudStats()
{
	if (!UserCloudStats) {
		APlayerController* LocalPlayerController = Cast<APlayerController>(GetOwner());

		if (!LocalPlayerController) { return; }

		if (!UserCloudStats && !LocalPlayerController->IsPrimaryPlayer()) {
			UserCloudStats = NewObject<UConceptUserCloudStats>(this, UConceptUserCloudStats::StaticClass());
//			UserCloudStats->InitStats(Cast<AConceptPlayerState>(this));

			return;
		}
		else if (UserCloudStats) {
			return;
		}


		UserCloudStats = Cast<UConceptUserCloudStats>(UGameplayStatics::LoadGameFromSlot("UserCloudStats", 0));

		if (!UserCloudStats) {
			UserCloudStats = NewObject<UConceptUserCloudStats>(this, UConceptUserCloudStats::StaticClass());
			//UserCloudStats->InitStats(Cast<AConceptPlayerState>(this));
			UGameplayStatics::SaveGameToSlot(UserCloudStats, "UserCloudStats", 0);
		}
		else {
		//	UserCloudStats->InitStats(Cast<AConceptPlayerState>(this));
		}
		//return;


		UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
		GameInstance->ReadFileOfUserCloud("UserCloudStats.sav", LocalPlayerController);
	}
}

//void ASteamPlayerState::InitSteamStats_Multicast_Implementation()
//{
//	InitSteamStats();
//}
//
//void ASteamPlayerState::InitSteamStats_Server_Implementation()
//{
//	InitSteamStats_Multicast();
//}

//float ASteamPlayerState::GetStatValue(ESteamStats StatName)
//{
//	//FName Name = SteamStatEnumToFName(StatName);
//	//if (!SteamStats)
//	//	return -1.0f;
//
//	//return SteamStats->GetStatValue(Name);
//
//	return -1;
//}
//
//FString ASteamPlayerState::GetStatInfo(ESteamStats Name)
//{
//
//	//if (!SteamStats)
//	//	return "ERROR";
//
//	//return SteamStats->GetStatInfo(SteamStatEnumToFName(Name));
//
//	return "NULL";
//}


//FName ASteamPlayerState::SteamStatEnumToFName(ESteamStats Name){
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESteamStats"), true);
//	if (!EnumPtr) return "Invalid";
//
//	FString ToReturn = EnumPtr->GetEnumName((int32)Name);
//	ToReturn = ToReturn.Mid(3);
//	FName ConvertedFString = FName(*ToReturn);
//	return ConvertedFString;
//}

//ESteamStats ASteamPlayerState::FNameToSteamStatEnum(FName Name) {
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESteamStats"), true);
//
//	int32 Index = EnumPtr->FindEnumIndex(*Name.ToString());
//
//	return (ESteamStats)EnumPtr->FindEnumIndex(*Name.ToString());
//}

//void ASteamPlayerState::WriteLocalStat_Implementation(ESteamStats StatName, float Value) {
//	FName Name = SteamStatEnumToFName(StatName);
//
//	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("WriteLocalStat: %s - %f"), *Name.ToString() ,Value));
//	//if (SteamStats) {
//	//	SteamStats->WriteLocalStat(Name, Value);
//	//}
//}

//void ASteamPlayerState::WriteIncrementLocalStat_Implementation(ESteamStats StatName, float Increment) {
//	FName Name = SteamStatEnumToFName(StatName);
//
//	//if (SteamStats) {
//	//	float LocalValue = SteamStats->GetStatValue(Name);
//	//	SteamStats->WriteLocalStat(Name, LocalValue + Increment);
//	//}
//}


//void ASteamPlayerState::WriteStat_Implementation(ESteamStats StatName, float Value) {
//	FName Name = SteamStatEnumToFName(StatName);
//
//	//if (SteamStats) {
//	//	SteamStats->WriteLocalStat(Name, Value);
//	//	SteamStats->UpdateStat(Name);
//	//}
//}

//void ASteamPlayerState::WriteAllStats_Implementation() {
//	//if (SteamStats) {
//	//	SteamStats->UpdateAllStatsToSteam();
//	//}
//}

//void ASteamPlayerState::WriteLeaderboardStat_Implementation(ESteamStats StatName, float Value) {
//	FName Name = SteamStatEnumToFName(StatName);
//
//	//if (SteamStats) {
//	//	SteamStats->WriteLeaderboardStat(SessionName, Name, Value);
//	//}
//}

//void ASteamPlayerState::WriteIncrementStat_Implementation(ESteamStats StatName, float Increment) {
//	FName Name = SteamStatEnumToFName(StatName);
//
//	//if (SteamStats) {
//	//	float LocalValue = SteamStats->GetStatValue(Name);
//	//	SteamStats->WriteLocalStat(Name, LocalValue + Increment);
//	//	SteamStats->UpdateStat(Name);
//	//}
//}
//void ASteamPlayerState::WriteIncrementLeaderboardStat_Implementation(ESteamStats StatName) {
//	FName Name = SteamStatEnumToFName(StatName);
//
//	//if (SteamStats) {
//	//	float LocalValue = SteamStats->GetStatValue(Name);
//	//	SteamStats->WriteLeaderboardStat(SessionName, Name, LocalValue + 1);
//	//}
//}


//bool ASteamPlayerState::WriteIncrementStat_Validate(ESteamStats StatName, float Increment) { return true; }
//bool ASteamPlayerState::WriteIncrementLeaderboardStat_Validate(ESteamStats StatName) { return true; }
//bool ASteamPlayerState::WriteLocalStat_Validate(ESteamStats StatName, float Value) {return true;}
//bool ASteamPlayerState::WriteIncrementLocalStat_Validate(ESteamStats StatName, float Increment) {return true;}
//bool ASteamPlayerState::WriteStat_Validate(ESteamStats StatName, float Value) { return true; }
//bool ASteamPlayerState::WriteAllStats_Validate() { return true; }
//bool ASteamPlayerState::WriteLeaderboardStat_Validate(ESteamStats StatName, float Value) { return true; }
//bool ASteamPlayerState::InitSteamStats_Multicast_Validate() {return true;}
//bool ASteamPlayerState::InitSteamStats_Server_Validate() {return true;}