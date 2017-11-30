// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "D_Stun.h"


UD_Stun::UD_Stun()
{

	BaseDamageColor = FLinearColor::White;
	LowDamageColor = FLinearColor::White;
	MedDamageColor = FLinearColor::White;
	HighDamageColor = FLinearColor::White;
}

void UD_Stun::DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior)
{
	AConceptPlayerController* KilledController = Cast<AConceptPlayerController>(KilledPlayer);
	if (KilledController) {
		AConceptCharacter* Character = Cast<AConceptCharacter>(KilledPlayer->GetPawn());
	}
	DamageBehavior->ApplyLifeDamage(Damage, this->GetClass());
}

void UD_Stun::DamageEffect(float Duration, float Damage, UConceptDamageBehavior* DamageBehavior){
    DamageBehavior->ApplyStunDamage(Duration);
}
