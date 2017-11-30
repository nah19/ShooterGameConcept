// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_BetterThanYou.h"


UM_BetterThanYou::UM_BetterThanYou()
{
	MedalName = NSLOCTEXT("Medals", "UM_BetterThanYou", "Mejor que tu");
	Description = NSLOCTEXT("Medals", "UM_BetterThanYou_Des", "Consigue entre 20 y 40 asesinatos");

	Multiplier = 1.2;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/KillStrike__5__Symbol-01.KillStrike__5__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 4;
}

UMedal* UM_BetterThanYou::GetAchievedMedal(AConceptStats* Stats){
	if (Stats->Kills >= 20 && Stats->Kills < 40) {
		return this;
	}

	return NULL;
}



