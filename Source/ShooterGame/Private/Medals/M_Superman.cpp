// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Superman.h"



UM_Superman::UM_Superman()
{
	MedalName = NSLOCTEXT("Medals", "UM_Superman", "SUPERMAN");
	Description = NSLOCTEXT("Medals", "UM_Superman_Des", "Pasa la mayor parte del tiempo en el aire");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Most_time_flying_Symbol-01.Most_time_flying_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 28;
}

UMedal* UM_Superman::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->TimeFlying > Stats->TimeSprinting && Stats->TimeFlying > Stats->TimeCrouched && Stats->TimeFlying > Stats->TimeWalking) {
		return this;
	}

	return NULL;
}


