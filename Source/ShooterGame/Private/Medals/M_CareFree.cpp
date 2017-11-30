// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_CareFree.h"


UM_CareFree::UM_CareFree()
{
	MedalName = NSLOCTEXT("Medals", "UM_CareFree", "SIN CUIDADO");
	Description = NSLOCTEXT("Medals", "UM_CareFree_Des", "Se la primera muerte de la partida");

	Multiplier = 1.05;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Last_Kill_Symbol-01.Last_Kill_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 6;
}

UMedal* UM_CareFree::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->FirstDeath) {
		return this;
	}

	return NULL;
}

