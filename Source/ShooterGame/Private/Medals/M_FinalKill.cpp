// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_FinalKill.h"


UM_FinalKill::UM_FinalKill()
{
	MedalName = NSLOCTEXT("Medals", "UM_FinalKill", "Ultimo asesinato");
	Description = NSLOCTEXT("Medals", "UM_FinalKill_Des", "Ultimo asesinato de la partida");

	Multiplier = 1.05;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Last_Kill_Symbol-01.Last_Kill_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 9;
}

UMedal* UM_FinalKill::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->FinalKill) {
		return this;
	}

	return NULL;
}


