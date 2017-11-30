// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_FriendlyDeath.h"

UM_FriendlyDeath::UM_FriendlyDeath()
{
	MedalName = NSLOCTEXT("Medals", "UM_FriendlyDeath", "NO VOY A MATAR A UN AMIGO");
	Description = NSLOCTEXT("Medals", "UM_FriendlyDeath_Des", "No has asesinado a ningun amigo");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/No_Friendly_Death_Symbol-01.No_Friendly_Death_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 11;
}

UMedal* UM_FriendlyDeath::GetAchievedMedal(AConceptStats* Stats) {
	if (!Stats->FriendlyDeath) {
		return this;
	}

	return NULL;
}



