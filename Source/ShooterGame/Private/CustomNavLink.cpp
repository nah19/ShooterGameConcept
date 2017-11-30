// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "CustomNavLink.h"


ACustomNavLink::ACustomNavLink(const FObjectInitializer& ObjectInitializer) {
	if (PointLinks.Num() == 1) {
		LeftPoint = PointLinks[0].Left;
		RightPoint = PointLinks[0].Right;
	}
}

#if WITH_EDITOR
void ACustomNavLink::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!PropertyChangedEvent.Property)
		return;
	if (PointLinks.Num() == 1) {
		LeftPoint = PointLinks[0].Left;
		RightPoint = PointLinks[0].Right;
	}
}
#endif