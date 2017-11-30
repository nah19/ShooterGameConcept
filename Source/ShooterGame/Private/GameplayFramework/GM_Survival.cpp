// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "GM_Survival.h"
#include "PS_Survival.h"



AGM_Survival::AGM_Survival(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	WaitingHordeTime = 20;
	HordeTime = 600;
	ResultsTime = 30;
}

TSubclassOf<AGameSession> AGM_Survival::GetGameSessionClass() const
{
	return AConceptGameSession::StaticClass();
}


AGameSession* AGM_Survival::GetGameSession() {
	return GameSession;
}

void AGM_Survival::DefaultTimer() {
	if (MyGameState && MyGameState->RemainingTime > 0 && !MyGameState->bTimerPaused) {
		MyGameState->RemainingTime--;

		if (MyGameState->RemainingTime <= 0) {
			if (GetMatchState() == MatchState::WaitingPostMatch) {
				DestroySession();
				SyncronizingAllPlayerStateCharacters();
			}
			else if (GetMatchState() == MatchState::InProgress) {
				if (MyGameState->InProgressState == EInProgressStateEnum::RE_EARLYSTAGE) {
					HandleWaitingHorde();
				}
				else if (MyGameState->InProgressState == EInProgressStateEnum::RE_WAITING_HORDE) {
					HandleFightingHorde();
				}
				else if (MyGameState->InProgressState == EInProgressStateEnum::RE_FIGHTING_HORDE) {
					HandleResultsHorde();
				}
			}
			else if (GetMatchState() == MatchState::WaitingToStart) {
				StartMatch();
				//HandleMatchEarlyStage();
			}
		}
		else{
			if(MyGameState->InProgressState == EInProgressStateEnum::RE_FIGHTING_HORDE && Enemies.Num() == 0){
				Hordes[CurrentHordeIndex].FinalTime = Hordes[CurrentHordeIndex].RoundTime - MyGameState->RemainingTime;
				MyGameState->RemainingTime = 1;
			}
		}
	}
}

void AGM_Survival::HandleMatchHasStarted(){
	Super::HandleMatchHasStarted();
	//HandleMatchEarlyStage();
}

void AGM_Survival::HandleWaitingHorde_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("HandleWaitingHorde")));
	
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->GetClass() == PickUpClass) {
			ActorItr->Destroy();
		}
	}

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if(ActorItr->Tags.Find("Pickup") != -1){
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			GetWorld()->SpawnActor<AActor>(PickUpClass, ActorItr->GetActorLocation(), ActorItr->GetActorRotation(), SpawnParams);

		}
	}

	MyGameState->InProgressState = EInProgressStateEnum::RE_WAITING_HORDE;

	ReactiveAllCharacters();
	MyGameState->RemainingTime = WaitingHordeTime;

	SyncronizingAllPlayerStateCharacters();
}

void AGM_Survival::HandleFightingHorde_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("HandleFightingHorde")));
	MyGameState->InProgressState = EInProgressStateEnum::RE_FIGHTING_HORDE;

	MyGameState->RemainingTime = Hordes[CurrentHordeIndex].RoundTime;
	FillArrayOfEnemies();
	SpawnEnemies();

	SyncronizingAllPlayerStateCharacters();
}

void AGM_Survival::HandleResultsHorde_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("HandleResultsHorde")));
	MyGameState->InProgressState = EInProgressStateEnum::RE_RESULTS_HORDE;

	for (TActorIterator<APS_Survival> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		ActorItr->AddHorde(Hordes[CurrentHordeIndex]);
	}


	if(Hordes.Num() == (CurrentHordeIndex + 1)){
		ShowFinalResults();
	}

	if(((CurrentHordeIndex + 1) % 10) == 0){
		ShowFaseHordeResults();
	}
	else{
		ShowLightHordeResults();
		MyGameState->RemainingTime = 10;

		FTimerHandle Handle;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AGM_Survival::HandleWaitingHorde);
		GetWorldTimerManager().SetTimer(Handle, Delegate, 10.0f, false);
	}
	
	CurrentHordeIndex++;
}

void AGM_Survival::HandleMatchEarlyStage_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("HandleMatchEarlyStage_Implementation")));
	MyGameState->InProgressState = EInProgressStateEnum::RE_EARLYSTAGE;

	AConceptGameSession* MySession = Cast<AConceptGameSession>(GameSession);
	MySession->StartSession();

	MyGameState->RemainingTime = MyGameState->EarlyStageTime;
	SyncronizingAllPlayerStateCharacters();

}


void AGM_Survival::ShowFaseHordeResults_Implementation(){

}

void AGM_Survival::ShowLightHordeResults_Implementation(){

}

void AGM_Survival::ShowFinalResults_Implementation(){

}


void AGM_Survival::FillArrayOfEnemies(){
	EnemiesClasses.Empty();
	Enemies.Empty();
	
	FHordeRound LocalHorde = Hordes[CurrentHordeIndex];
	float Difficult = LocalHorde.DifficultyPoints;
	bool Restado = true;
	while (Restado) {
		for (int32 i = 0; i < LocalHorde.EnemiesClasses.Num(); i++) {
			float Random = FMath::RandRange(0, 9);
			if (LocalHorde.EnemiesWeight[i] != 0 && (LocalHorde.EnemiesWeight[i] / 10.0f) > Random) {
                
				
				float DifficultPts = 0;
                AMinionCharacter* GroundMinion = Cast<AMinionCharacter>(LocalHorde.EnemiesClasses[i]->GetDefaultObject());
                if(GroundMinion){
                    DifficultPts = GroundMinion->DifficultPoints;
                }else {
                    AYBlinnPawn* FlyMinion = Cast<AYBlinnPawn>(LocalHorde.EnemiesClasses[i]->GetDefaultObject());
                    if (FlyMinion){
                        DifficultPts = FlyMinion->DifficultPoints;
                    }
                }


				if (Difficult > DifficultPts) {
					EnemiesClasses.Add(LocalHorde.EnemiesClasses[i]);
					GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, FString::Printf(TEXT("FillArrayOfEnemies %f"), DifficultPts));
					Difficult -= DifficultPts;	
				}
				else {
					Restado = false;
				}
			}
		}
	}

	for (FActorIterator It(GetWorld()); It; ++It) {
		APS_Survival* LocalPlayerState = Cast<APS_Survival>(*It);
		if (LocalPlayerState) {
			LocalPlayerState->RoundEnemiesKilledClasses.Empty();
		}
	}

	MaxEnemies = EnemiesClasses.Num();
}

void AGM_Survival::ContinueSpawnEnemies() {
	if(Enemies.Num() < 5){
		SpawnEnemies();
	}
}

void AGM_Survival::SpawnEnemies(){
	double Distance = 1000000000;
	AConceptPlayerStart* NearestSpawnPoint = NULL;
	int32 i = 0;
	for (FActorIterator It(GetWorld()); It; ++It) {
		AConceptPlayerStart* TestSpawn = Cast<AConceptPlayerStart>(*It);
		APS_Survival* LocalPlayerState = Cast<APS_Survival>(*It);

		if (TestSpawn != NULL && i == 0) {
			NearestSpawnPoint = TestSpawn;
            i++;
		}

		if (TestSpawn != NULL && TestSpawn->PlayerStartTag == "Horde") {
			for (TActorIterator<AConceptCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
				if (Distance > FVector::Distance(ActorItr->GetActorLocation(), TestSpawn->GetActorLocation())){
					if (FVector::Distance(ActorItr->GetActorLocation(), TestSpawn->GetActorLocation()) > 5000) {
						Distance = FVector::Distance(ActorItr->GetActorLocation(), TestSpawn->GetActorLocation());
						NearestSpawnPoint = TestSpawn;
					}
				}
			}
		}
	}

	// don't allow pawn to be spawned with any pitch or roll
	FRotator StartRotation(ForceInit);
	StartRotation = NearestSpawnPoint->GetActorRotation();
	FVector StartLocation = NearestSpawnPoint->GetActorLocation();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	//SpawnInfo.bNoFail = true;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < (EnemiesClasses.Num() / 2); i++) {
		APawn* ResultPawn = NULL;
		FVector RandomVector = NearestSpawnPoint->GetActorForwardVector() * FMath::RandRange(-3000, 3000);
		ResultPawn = GetWorld()->SpawnActor<APawn>(EnemiesClasses[i], StartLocation + RandomVector, StartRotation, SpawnInfo);
		Enemies.Add(ResultPawn);
		EnemiesClasses.RemoveAt(i);
	}

	
}
