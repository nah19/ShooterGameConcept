// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_NoFear.h"




UM_NoFear::UM_NoFear()
{
	MedalName = NSLOCTEXT("Medals", "UM_NoFear", "SIN MIEDO");
	Description = NSLOCTEXT("Medals", "UM_NoFear_Des", "Consigue el primer asesinato");

	Multiplier = 1.05;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/First_Kill_Symbol-01.First_Kill_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 24;
}

UMedal* UM_NoFear::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->FirstKill) {
		return this;
	}

	return NULL;
}