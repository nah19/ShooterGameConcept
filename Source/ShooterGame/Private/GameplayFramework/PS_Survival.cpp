// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "PS_Survival.h"


APS_Survival::APS_Survival() {



}

void APS_Survival::BeginPlay(){
	Super::BeginPlay();

	//AConceptPlayerController* LocalController = Cast<AConceptPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//if (LocalController && LocalController->MyCharacter) {
	//	LocalController->MyCharacter->StaminaReg = LocalController->MyCharacter->StaminaReg * 0.1f;
	//}
}

void APS_Survival::SyncronizingPlayerStateInProgress(EInProgressStateEnum MatchState){

	switch (MatchState) {
	case EInProgressStateEnum::RE_EARLYSTAGE: {
		if (State == EStateEnum::RE_INITIAL_SPECTATOR) {
			SetPlayerState(EStateEnum::RE_BLOCKED);
		}
	}
											  break;
	case EInProgressStateEnum::RE_WAITING_HORDE: {
		if (State == EStateEnum::RE_BLOCKED) {
			SetPlayerState(EStateEnum::RE_PLAYING);
		}
	}
											   break;
	case EInProgressStateEnum::RE_FIGHTING_HORDE: {
		if (State == EStateEnum::RE_BLOCKED) {
			SetPlayerState(EStateEnum::RE_PLAYING);
		}
	}
												 break;
	case EInProgressStateEnum::RE_RESULTS_HORDE: {
		if (State == EStateEnum::RE_BLOCKED) {
			SetPlayerState(EStateEnum::RE_RESULTS);
		}
	}
												 break;
	}
}

void APS_Survival::AddHorde(FHordeRound Horde){
	HordesCompleted.Add(Horde);
}

TArray<int32> APS_Survival::ExperienceCalculator(){
	TArray<int32> LocalResults;
	LocalResults.Add(0);
	LocalResults.Add(0);
	LocalResults.Add(0);

	TArray<TSubclassOf<APawn>> EnemiesClasses;
	RoundEnemiesKilledClasses.GetKeys(EnemiesClasses);
	for(int32 i = 0; i<EnemiesClasses.Num(); i++){
		int32 ValueKey = *RoundEnemiesKilledClasses.Find(EnemiesClasses[i]);
        AMinionCharacter* GroundMinion = Cast<AMinionCharacter>(EnemiesClasses[i]->GetDefaultObject());
        float DifficultPts = 0;
        if(GroundMinion){
            DifficultPts = GroundMinion->DifficultPoints;
        }else {
            AYBlinnPawn* FlyingMinion = Cast<AYBlinnPawn>(EnemiesClasses[i]->GetDefaultObject());
            if(FlyingMinion){
                DifficultPts = FlyingMinion->DifficultPoints;
            }
        }
        
        LocalResults[0] += ValueKey * DifficultPts * 10;
		
	}

	LocalResults[2] = HordesCompleted[HordesCompleted.Num() - 1].RoundTime - HordesCompleted[HordesCompleted.Num() - 1].FinalTime;

	return LocalResults;
}

void APS_Survival::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Survival, HordesCompleted);
}
