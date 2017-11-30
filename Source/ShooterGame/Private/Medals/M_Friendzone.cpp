// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Friendzone.h"


UM_Friendzone::UM_Friendzone()
{
	MedalName = NSLOCTEXT("Medals", "UM_Friendzone", "FRIENDZONE");
	Description = NSLOCTEXT("Medals", "UM_Friendzone_Des", "Mas asistencias de la partida");

	Multiplier = 1.2;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Most_assists_Symbol-01.Most_assists_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 13;
}

UMedal* UM_Friendzone::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->MostAssists) {
		return this;
	}

	return NULL;
}




