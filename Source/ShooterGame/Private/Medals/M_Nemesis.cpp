// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Nemesis.h"



UM_Nemesis::UM_Nemesis()
{
	MedalName = NSLOCTEXT("Medals", "UM_Nemesis", "NEMESIS");
	Description = NSLOCTEXT("Medals", "UM_Nemesis_Des", "Se el Nemesis de un enemigo");

	Multiplier = 1.5;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Nemesis__5__Symbol-01.Nemesis__5__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 23;
}

UMedal* UM_Nemesis::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->Nemesis) {
		return this;
	}

	return NULL;
}


