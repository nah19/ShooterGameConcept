// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Calcuta.h"



UM_Calcuta::UM_Calcuta()
{
	MedalName = NSLOCTEXT("Medals", "UM_Calcuta", "CALCUTA");
	Description = NSLOCTEXT("Medals", "UM_Calcuta_Des", "Asiste mas que asesina");

	Multiplier = 1.2;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/More_Assists_than_Kills_Symbol-01.More_Assists_than_Kills_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 5;
}

UMedal* UM_Calcuta::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->Kills < Stats->Assist) {
		return this;
	}

	return NULL;
}