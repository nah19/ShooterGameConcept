// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_God.h"

UM_God::UM_God()
{
	MedalName = NSLOCTEXT("Medals", "UM_God", "DIOS");
	Description = NSLOCTEXT("Medals", "UM_God_Des", "Racha de mas de 20 asesinatos");

	Multiplier = 1.4;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/KillStrike__20__Symbol-01.KillStrike__20__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 14;
}


UMedal* UM_God::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->KillStrike >= 20) {
		return this;
	}

	return NULL;
}

