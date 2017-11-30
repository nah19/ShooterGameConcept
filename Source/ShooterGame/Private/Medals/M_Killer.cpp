// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Killer.h"

UM_Killer::UM_Killer()
{
	MedalName = NSLOCTEXT("Medals", "UM_Killer", "ASESINO");
	Description = NSLOCTEXT("Medals", "UM_Killer_Des", "Racha de entre 15 y 20 asesinatos");

	Multiplier = 1.3;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/KillStrike__15__Symbol-01.KillStrike__15__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 18;
}


UMedal* UM_Killer::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->KillStrike >= 15 && Stats->KillStrike < 20) {
		return this;
	}

	return NULL;
}
