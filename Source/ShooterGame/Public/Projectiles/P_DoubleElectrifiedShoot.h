// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Projectiles/Projectile.h"
#include "P_DoubleElectrifiedShoot.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_DoubleElectrifiedShoot : public AProjectile
{
	GENERATED_BODY()
	
public:
    AP_DoubleElectrifiedShoot(const FObjectInitializer& ObjectInitializer);
	
    virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit);
    
    
};
