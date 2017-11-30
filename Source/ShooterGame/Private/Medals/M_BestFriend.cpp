// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_BestFriend.h"

UM_BestFriend::UM_BestFriend()
{
	MedalName = NSLOCTEXT("Medals", "UM_BestFriend", "EL MEJOR AMIGO");
	Description = NSLOCTEXT("Medals", "UM_BestFriend_Des", "Consigue mas de 10 asistencias");
	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Assists__10__Symbol-01.Assists__10__Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 3;
}

UMedal* UM_BestFriend::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->Assist >= 10) {
		return this;
	}

	return NULL;
}



