// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_AfterDeath.h"

UM_AfterDeath::UM_AfterDeath()
{

		MedalName = NSLOCTEXT("Medals", "UM_AfterDeath", "Hay algo despues de la muerte?");
		Description = NSLOCTEXT("Medals", "UM_AfterDeath_Des", "Asesina a alguien despues de morir");
		Multiplier = 1.2;
		Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Kill_enemy_after_die_Symbol-01.Kill_enemy_after_die_Symbol-01"), NULL, LOAD_None, NULL);
		
		TypeIndex = 1;
}

UMedal* UM_AfterDeath::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->AfterDeath) {
		return this;
	}

	return NULL;
}



