// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "D_BleedingStatus.h"





UD_BleedingStatus::UD_BleedingStatus() {

	BaseDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	LowDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	MedDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	HighDamageColor = FLinearColor::FLinearColor(230, 0, 255);
}

void UD_BleedingStatus::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior) {
	AConceptPlayerController* KilledController = Cast<AConceptPlayerController>(KilledPlayer);

	if (KilledController) {
		//KilledController->InitInGameDamageHUD();
		AConceptCharacter*  KilledCharacter = Cast<AConceptCharacter>(KilledController->GetPawn());
		if (KilledCharacter) {
			KilledCharacter->ShakeManager(CameraShakeClass, false, Damage / 10);
		}
	}

	AConceptPlayerController* KillerController = Cast<AConceptPlayerController>(KillerPlayer);

	DamageBehavior->ApplyLifeDamage(Damage, this->GetClass());
	DamageBehavior->ApplyPoisonDamage(KillerPlayer, Damage, 15, 1);
}

