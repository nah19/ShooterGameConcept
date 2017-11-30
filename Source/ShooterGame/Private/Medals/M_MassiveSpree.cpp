// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_MassiveSpree.h"

UM_MassiveSpree::UM_MassiveSpree()
{
	MedalName = NSLOCTEXT("Medals", "UM_MassiveSpree", "RACHA MASIVA");
	Description = NSLOCTEXT("Medals", "UM_MassiveSpree_Des", "Racha de entre 10 y 15 asesinatos");

	Multiplier = 1.2;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/KillStrike__10__Symbol-01.KillStrike__10__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 22;
}

UMedal* UM_MassiveSpree::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->KillStrike >= 10 && Stats->KillStrike < 15) {
		return this;
	}

	return NULL;
}

