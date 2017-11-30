// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Helper.h"



UM_Helper::UM_Helper()
{
	MedalName = NSLOCTEXT("Medals", "UM_Helper", "HELPER");
	Description = NSLOCTEXT("Medals", "UM_Helper_Des", "Mas de 5 asistencias");

	Multiplier = 1.05;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Assists__5__Symbol-01.Assists__5__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 16;
}

UMedal* UM_Helper::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->Assist >= 5) {
		return this;
	}

	return NULL;
}
