// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Redemption.h"



UM_Redemption::UM_Redemption()
{
	MedalName = NSLOCTEXT("Medals", "UM_Redemption", "REDENCION");
	Description = NSLOCTEXT("Medals", "UM_Redemption_Des", "Asesina a tu nemesis");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Kill_Nemesis_Symbol-01.Kill_Nemesis_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 26;
}

UMedal* UM_Redemption::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->NemesisVendeta) {
		return this;
	}

	return NULL;
}


