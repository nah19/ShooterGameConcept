// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "P_Bouncing.h"


AP_Bouncing::AP_Bouncing(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void AP_Bouncing::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	if (NumberOfRebounds < 0) Destroy();
	NumberOfRebounds--;
	APawn* Character = Cast<APawn>(GetOwner());
	APawn* Enemy = Cast<APawn>(ActorToAffectParam);

	if (Character && Enemy) {
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
		Destroy();
	}
}

