// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "D_ConstrictorDamage.h"


UD_ConstrictorDamage::UD_ConstrictorDamage() {


	BaseDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	LowDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	MedDamageColor = FLinearColor::FLinearColor(230, 0, 255);
	HighDamageColor = FLinearColor::FLinearColor(230, 0, 255);
}

void UD_ConstrictorDamage::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior) {
	AConceptPlayerController* KilledController = Cast<AConceptPlayerController>(KilledPlayer);
	if (KilledController) {
		AConceptCharacter*  KilledCharacter = Cast<AConceptCharacter>(KilledController->GetPawn());
		if (KilledCharacter) {
			KilledCharacter->ShakeManager(CameraShakeClass, false, Damage / 10);
		}
	}

	AConceptPlayerController* KillerController = Cast<AConceptPlayerController>(KillerPlayer);

	DamageBehavior->ApplyLifeDamage(Damage, this->GetClass());
	DamageBehavior->ApplyConstrictionDamage(KillerPlayer, Damage, 2, 1);


}