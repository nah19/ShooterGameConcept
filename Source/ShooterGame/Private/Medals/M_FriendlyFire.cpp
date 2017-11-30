// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_FriendlyFire.h"


UM_FriendlyFire::UM_FriendlyFire()
{
	MedalName = NSLOCTEXT("Medals", "UM_FriendlyFire", "DE CONFIANZA");
	Description = NSLOCTEXT("Medals", "UM_FriendlyFire_Des", "Nunca has herido a un companyero");

	Multiplier = 1.1;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/No_Friendly_Damage_Symbol-01.No_Friendly_Damage_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 12;
}

UMedal* UM_FriendlyFire::GetAchievedMedal(AConceptStats* Stats) {
	if (!Stats->FriendlyFire) {
		return this;
	}

	return NULL;
}


