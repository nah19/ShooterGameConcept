// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "M_Vendetta.h"


UM_Vendetta::UM_Vendetta()
{
	MedalName = NSLOCTEXT("Medals", "UM_Vendetta", "VENDETTA");
	Description = NSLOCTEXT("Medals", "UM_Vendetta_Des", "Asesina al ultimo enemigo que te mato");

	Multiplier = 1.05;
	Image = LoadObject<UTexture2D>(NULL, TEXT("/Game/Textures/MenusAndHUD/Medals/Icon/Revenge_Symbol-01.Revenge_Symbol-01"), NULL, LOAD_None, NULL);
	TypeIndex = 29;
}

UMedal* UM_Vendetta::GetAchievedMedal(AConceptStats* Stats) {
	if (Stats->KillLastEnemyWhoKilledYou) {
		return this;
	}

	return NULL;
}


