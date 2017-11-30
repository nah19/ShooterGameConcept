// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Granma.h"


UM_Granma::UM_Granma()
{
	MedalName = NSLOCTEXT("Medals", "UM_Granma", "ABUELITA");
	Description = NSLOCTEXT("Medals", "UM_Granma_Des", "Agachado la mayoria del tiempo");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Kill_enemy_after_die_Symbol-01.Kill_enemy_after_die_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 15;
}

UMedal* UM_Granma::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->TimeCrouched > Stats->TimeSprinting && Stats->TimeCrouched > Stats->TimeFlying && Stats->TimeCrouched > Stats->TimeWalking) {
		return this;
	}

	return NULL;
}


