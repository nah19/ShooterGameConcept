// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "ConceptUserCloudStats.h"



UConceptUserCloudStats::UConceptUserCloudStats()
{
	for (int32 i = 0; i < 31; i++) {
		MedalsIndexQuantity.Add(i, 0);
	}

		Kills = 0;
		Deaths = 0;
		Assist = 0;
		Dashes = 0;
		KillStrike = 0;
		NumMatches = 0;
		NumMatchesW = 0;
		NumMatchesL = 0;
		Experience = 0;
		MedalsQuantity.Empty();
		Rolls = 0;
		Teleports = 0;
		DamageReceived = 0;
		HillsConquered = 0;
		TimeGameplay = 0;
		TimeSprinting = 0;
		TimeFlying = 0;
		Teamwork = 0;
		DamageDone = 0;
		Jumps = 0;
		ELO = 1500;
		GAMEVERSION = 950;
		Level = 0;
}

void UConceptUserCloudStats::Init(AConceptStats* ConceptStatsParam) {
	ConceptStats = ConceptStatsParam;
}

void UConceptUserCloudStats::GetFromConceptStats(AConceptStats* ConceptStats) {



	Kills += ConceptStats->Kills;
	Deaths += ConceptStats->Deaths;
	Assist += ConceptStats->Assist;
	Dashes += ConceptStats->Dashes;
	KillStrike = ConceptStats->KillStrike;
	NumMatches += ConceptStats->NumMatches;
	NumMatchesW += ConceptStats->NumMatchesW;
	NumMatchesL += ConceptStats->NumMatchesL;
	Experience += ConceptStats->Experience;
	Rolls += ConceptStats->Rolls;
	Teleports += ConceptStats->Teleports;
	DamageReceived += ConceptStats->Damage;
	HillsConquered += ConceptStats->HillsConquered;
	TimeGameplay += ConceptStats->TimeGameplay;
	TimeSprinting += ConceptStats->TimeSprinting;
	TimeFlying += ConceptStats->TimeFlying;
	Teamwork += ConceptStats->Teamwork;
	DamageDone += ConceptStats->DamageDone;
	Jumps += ConceptStats->Jumps;
	ELO = ConceptStats->ELO;


	MedalsIndex.Empty();
	MedalsQuantity.Empty();

	for (int32 i = 0; i < ConceptStats->Medals.Num(); i++) {
		//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Medals LOOP")));
		if (ConceptStats->Medals[i]) {
			//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("MedalName %s"), *ConceptStats->Medals[i]->MedalName.ToString()));
			int32 Index = ConceptStats->Medals[i]->TypeIndex;
			int32 CurrentValue = MedalsIndexQuantity.FindRef(Index);
			CurrentValue++;
			MedalsIndexQuantity.Add(Index, CurrentValue);
		}
	}

	MedalsIndexQuantity.GetKeys(MedalsIndex);
	MedalsIndexQuantity.GenerateValueArray(MedalsQuantity);



}

void UConceptUserCloudStats::PushToConceptStats(AConceptStats* ConceptStats) {
	ConceptStats->InitialELO = ELO;
}



void UConceptUserCloudStats::ToString() {
	for (int32 i = 0; i < MedalsIndex.Num(); i++) {
		//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("INDEX: %d - QUANTITY: %d"), MedalsIndex[i], MedalsQuantity[i]));
	}


	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Kills %d"), Kills));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Deaths %d"), Deaths));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Assist %d"), Assist));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Dashes %d"), Dashes));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("KillStrike %d"), KillStrike));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("NumMatches %d"), NumMatches));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("NumMatchesW %d"), NumMatchesW));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("NumMatchesL %d"), NumMatchesL));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Experience %d"), Experience));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Rolls %d"), Rolls));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Teleports %d"), Teleports));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("DamageReceived %d"), DamageReceived));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("HillsConquered %d"), HillsConquered));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("TimeGameplay %f"), TimeGameplay));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("TimeSprinting %f"), TimeSprinting));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("TimeFlying %f"), TimeFlying));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Teamwork %d"), Teamwork));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("DamageDone %d"), DamageDone));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Jumps %d"), Jumps));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("ELO %d"), ELO));

}

FString UConceptUserCloudStats::GetNameOfStat(ESteamStats Name) {
	switch (Name)
	{
	case ESteamStats::RE_Assists: { return NSLOCTEXT("UserCloudStats", "RE_Assists", "Asistencias").ToString(); }
	case ESteamStats::RE_Best_Kill_Strike: { return NSLOCTEXT("UserCloudStats", "RE_Best_Kill_Strike", "Mejor Racha").ToString(); }
	case ESteamStats::RE_Damage_Done: { return NSLOCTEXT("UserCloudStats", "RE_Damage_Done", "Danyo Infligido").ToString(); }
	case ESteamStats::RE_Damage_Received: { return NSLOCTEXT("UserCloudStats", "RE_Damage_Received", "Danyo Recibido").ToString(); }
	case ESteamStats::RE_Dashes: { return NSLOCTEXT("UserCloudStats", "RE_Dashes", "Dashes").ToString(); }
	case ESteamStats::RE_Deaths: { return NSLOCTEXT("UserCloudStats", "RE_Deaths", "Muertes").ToString(); }
	case ESteamStats::RE_ELO: { return NSLOCTEXT("UserCloudStats", "RE_ELO", "ELO").ToString(); }
	case ESteamStats::RE_Experience: { return NSLOCTEXT("UserCloudStats", "RE_Experience", "Experiencia").ToString(); }
	case ESteamStats::RE_Game_Version: { return NSLOCTEXT("UserCloudStats", "RE_Game_Version", "Version").ToString(); }
	case ESteamStats::RE_Hills_Conquered: { return NSLOCTEXT("UserCloudStats", "RE_Hills_Conquered", "Colinas Conquistadas").ToString(); }
	case ESteamStats::RE_Jumps: { return NSLOCTEXT("UserCloudStats", "RE_Jumps", "Saltos").ToString(); }
	case ESteamStats::RE_Kills: { return NSLOCTEXT("UserCloudStats", "RE_Kills", "Asesinatos").ToString(); }
	case ESteamStats::RE_Level: { return NSLOCTEXT("UserCloudStats", "RE_Level", "Nivel").ToString(); }
	case ESteamStats::RE_Num_Matches: { return NSLOCTEXT("UserCloudStats", "RE_Num_Matches", "Partidas").ToString(); }
	case ESteamStats::RE_Num_Matches_L: { return NSLOCTEXT("UserCloudStats", "RE_Num_Matches_L", "Partidas Perdidas").ToString(); }
	case ESteamStats::RE_Num_Matches_W: { return NSLOCTEXT("UserCloudStats", "RE_Num_Matches_W", "Partidas Ganadas").ToString(); }
	case ESteamStats::RE_Rolls: { return NSLOCTEXT("UserCloudStats", "RE_Rolls", "Volteretas").ToString(); }
	case ESteamStats::RE_Teamwork: { return NSLOCTEXT("UserCloudStats", "RE_Teamwork", "Juego en equipo").ToString(); }
	case ESteamStats::RE_Teleports: { return NSLOCTEXT("UserCloudStats", "RE_Teleports", "Teletransportes").ToString(); }
	case ESteamStats::RE_Time_Flying: { return NSLOCTEXT("UserCloudStats", "RE_Time_Flying", "Tiempo Volando").ToString(); }
	case ESteamStats::RE_Time_Gameplay: { return NSLOCTEXT("UserCloudStats", "RE_Time_Gameplay", "Tiempo de Juego").ToString(); }
	case ESteamStats::RE_Time_Sprinting: { return NSLOCTEXT("UserCloudStats", "RE_Time_Sprinting", "Tiempo Sprintando").ToString(); }


	default:
		break;
	}

	return "";
}

float UConceptUserCloudStats::GetValueOfStat(ESteamStats Name) {
	
	switch (Name)
	{
	case ESteamStats::RE_Assists:{ return Assist; }
	case ESteamStats::RE_Best_Kill_Strike: { return KillStrike; }
	case ESteamStats::RE_Damage_Done: { return DamageDone; }
	case ESteamStats::RE_Damage_Received: { return DamageReceived; }
	case ESteamStats::RE_Dashes: { return Dashes; }
	case ESteamStats::RE_Deaths: { return Deaths; }
	case ESteamStats::RE_ELO: { return ELO; }
	case ESteamStats::RE_Experience: { return Experience; }
	case ESteamStats::RE_Game_Version: { return GAMEVERSION; }
	case ESteamStats::RE_Hills_Conquered: { return HillsConquered; }
	case ESteamStats::RE_Jumps: { return Jumps; }
	case ESteamStats::RE_Kills: { return Kills; }
	case ESteamStats::RE_Level: { return Level; }
	case ESteamStats::RE_Num_Matches: { return NumMatches; }
	case ESteamStats::RE_Num_Matches_L: { return NumMatchesL; }
	case ESteamStats::RE_Num_Matches_W: { return NumMatchesW; }
	case ESteamStats::RE_Rolls: { return Rolls; }
	case ESteamStats::RE_Teamwork: { return Teamwork; }
	case ESteamStats::RE_Teleports: { return Teleports; }
	case ESteamStats::RE_Time_Flying: { return TimeFlying; }
	case ESteamStats::RE_Time_Gameplay: { return TimeGameplay; }
	case ESteamStats::RE_Time_Sprinting: { return TimeSprinting; }


	default:
		break;
	}

	return 0.0;
}