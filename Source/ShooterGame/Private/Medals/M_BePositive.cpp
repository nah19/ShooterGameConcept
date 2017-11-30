// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_BePositive.h"


UM_BePositive::UM_BePositive()
{

	MedalName = NSLOCTEXT("Medals", "UM_BePositive", "SE POSITIVO");
	Description = NSLOCTEXT("Medals", "UM_BePositive_Des", "Consigue un ratio positivo asesinatos/muertes");
	Multiplier = 1.2;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Positive_ratio_Symbol-01.Positive_ratio_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 2;
}

UMedal* UM_BePositive::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->Kills > Stats->Deaths) {
		return this;
	}

	return NULL;
}


