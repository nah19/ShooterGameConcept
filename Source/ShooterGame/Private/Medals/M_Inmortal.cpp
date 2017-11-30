// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Inmortal.h"


UM_Inmortal::UM_Inmortal()
{
	MedalName = NSLOCTEXT("Medals", "UM_Inmortal", "INMORTAL");
	Description = NSLOCTEXT("Medals", "UM_Inmortal_Des", "No morir en toda la partida");

	Multiplier = 1.5;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/No_die__Perfect__Symbol-01.No_die__Perfect__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 17;
}

UMedal* UM_Inmortal::GetAchievedMedal(AConceptStats* Stats){
	if (Stats->Deaths == 0) {
		return this;
	}

	return NULL;
}



