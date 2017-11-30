// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Massive.h"


UM_Massive::UM_Massive()
{
	MedalName = NSLOCTEXT("Medals", "UM_Massive", "MASIVO");
	Description = NSLOCTEXT("Medals", "UM_Massive_Des", "Mas de 40 asesinatos");

	Multiplier = 1.4;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/KillStrike__5__Symbol-01.KillStrike__5__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 21;
}

UMedal* UM_Massive::GetAchievedMedal(AConceptStats* Stats){
	if (Stats->Kills >= 40) {
		return this;
	}

	return NULL;
}



