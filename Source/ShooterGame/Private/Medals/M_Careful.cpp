// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Careful.h"



UM_Careful::UM_Careful()
{
	MedalName = NSLOCTEXT("Medals", "UM_Careful", "CUIDADOSO");
	Description = NSLOCTEXT("Medals", "UM_Careful_Des", "Se el jugador con menor danyo recibido");

	Multiplier = 2;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Take_the_least_amount_of_damage_Symbol-01.Take_the_least_amount_of_damage_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 7;
}

UMedal* UM_Careful::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->LessDamage) {
		return this;
	}

	return NULL;
}
