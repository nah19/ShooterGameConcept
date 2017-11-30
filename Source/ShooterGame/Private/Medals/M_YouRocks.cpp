// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_YouRocks.h"


UM_YouRocks::UM_YouRocks()
{
	MedalName = NSLOCTEXT("Medals", "UM_YouRocks", "TU MOLAS");
	Description = NSLOCTEXT("Medals", "UM_YouRocks_Des", "El mejor ratio asesinatos/muertes");

	Multiplier = 2;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Higthest_Ratio_Symbol-01.Higthest_Ratio_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 30;
}

UMedal* UM_YouRocks::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->HightestRatio) {
		return this;
	}

	return NULL;
}


