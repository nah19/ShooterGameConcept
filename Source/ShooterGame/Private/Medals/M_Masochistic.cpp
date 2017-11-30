// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Masochistic.h"


UM_Masochistic::UM_Masochistic()
{
	MedalName = NSLOCTEXT("Medals", "UM_Masochistic", "MASOQUISTA");
	Description = NSLOCTEXT("Medals", "UM_Masochistic_Des", "Suicidate");

	Multiplier = 1.2;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Kill_yourself_3_times_Symbol-01.Kill_yourself_3_times_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 20;
}

UMedal* UM_Masochistic::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->KillYourself >= 1) {
		return this;
	}

	return NULL;
}


