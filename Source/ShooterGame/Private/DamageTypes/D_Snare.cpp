// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "D_Snare.h"


UD_Snare::UD_Snare()
{

	BaseDamageColor = FLinearColor::White;
	LowDamageColor = FLinearColor::White;
	MedDamageColor = FLinearColor::White;
	HighDamageColor = FLinearColor::White;
}

void UD_Snare::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior)
{
	AConceptPlayerController* KilledController = Cast<AConceptPlayerController>(KilledPlayer);
	if (KilledController) {
		AConceptCharacter* Character = Cast<AConceptCharacter>(KilledPlayer->GetPawn());

		Character->StartSnareStatus(5.0f);
	}
	DamageBehavior->ApplyLifeDamage(Damage, this->GetClass());
}


