// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_RangerTexas.h"


UM_RangerTexas::UM_RangerTexas()
{
	MedalName = NSLOCTEXT("Medals", "UM_RangerTexas", "RANGER TEXAS");
	Description = NSLOCTEXT("Medals", "UM_RangerTexas_Des", "Anda o corre durante la mayoria del tiempo");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Kill_enemy_after_die_Symbol-01.Kill_enemy_after_die_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 25;
}

UMedal* UM_RangerTexas::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->TimeWalking > Stats->TimeSprinting && Stats->TimeWalking > Stats->TimeFlying && Stats->TimeWalking > Stats->TimeCrouched) {
		return this;
	}

	return NULL;
}

