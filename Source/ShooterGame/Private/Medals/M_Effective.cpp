// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Effective.h"


UM_Effective::UM_Effective()
{
	MedalName = NSLOCTEXT("Medals", "UM_Effective", "EFECTIVO");
	Description = NSLOCTEXT("Medals", "UM_Effective_Des", "Entre 10 y 20 asesinatos");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/KillStrike__5__Symbol-01.KillStrike__5__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 8;
}

UMedal* UM_Effective::GetAchievedMedal(AConceptStats* Stats){
	if (Stats->Deaths >= 10 && Stats->Deaths < 20) {
		return this;
	}

	return NULL;
}



