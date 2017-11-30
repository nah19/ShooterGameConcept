// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_LetMeOne.h"


UM_LetMeOne::UM_LetMeOne()
{
	MedalName = NSLOCTEXT("Medals", "UM_LetMeOne", "DEJAME UNO, POR FAVOR");
	Description = NSLOCTEXT("Medals", "UM_LetMeOne_Des", "Mas de 15 asistencias");

	Multiplier = 1.15;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Assists__15__Symbol-01.Assists__15__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 19;
}

UMedal* UM_LetMeOne::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->Assist >= 15) {
		return this;
	}

	return NULL;
}

