// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Flash.h"

UM_Flash::UM_Flash()
{
	MedalName = NSLOCTEXT("Medals", "UM_Flash", "FLASH");
	Description = NSLOCTEXT("Medals", "UM_Flash_Des", "La mayoria del tiempo sprintando");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Most_Time_Running_Symbol-01.Most_Time_Running_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 10;
}

UMedal* UM_Flash::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->TimeSprinting > Stats->TimeCrouched && Stats->TimeSprinting > Stats->TimeFlying && Stats->TimeSprinting > Stats->TimeWalking) {
		return this;
	}

	return NULL;
}



