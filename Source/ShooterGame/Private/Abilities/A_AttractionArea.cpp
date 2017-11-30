// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "A_AttractionArea.h"
#include "P_AttractionArea.h"



void AA_AttractionArea::StartAbility(){
	for (TActorIterator<AP_AttractionArea> AttractionAreas(GetWorld()); AttractionAreas; ++AttractionAreas) {
		AP_AttractionArea *AttractionArea = *AttractionAreas;
		if (AttractionArea) {
			AttractionArea->Deploy();
			return;
		}
	}
	Super::StartAbility();
	//if (PrimaryAbilityProjectiles.Num() > 0) {
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("startability atraction area if num"));
	//	for (AProjectile* Projectile : PrimaryAbilityProjectiles)
	//	{
	//		AP_AttractionArea* AttractionArea = Cast<AP_AttractionArea>(Projectile);
	//		if (AttractionArea) AttractionArea->Deploy();

	//	}
	//	PrimaryAbilityProjectiles.Empty();
	//}
	//else {
	//	Super::StartAbility();
	//}
}
