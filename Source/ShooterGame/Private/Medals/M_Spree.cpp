// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Spree.h"


UM_Spree::UM_Spree()
{
	MedalName = NSLOCTEXT("Medals", "UM_Spree", "RACHA");
	Description = NSLOCTEXT("Medals", "UM_Spree_Des", "Consigue una racha de entre 5 y 10 asesinatos");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/KillStrike__5__Symbol-01.KillStrike__5__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 27;
}

UMedal* UM_Spree::GetAchievedMedal(AConceptStats* Stats){
	if (Stats->KillStrike >= 5 && Stats->KillStrike < 10) {
		return this;
	}

	return NULL;
}



