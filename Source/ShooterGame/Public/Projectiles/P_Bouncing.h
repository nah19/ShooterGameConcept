// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Projectiles/Projectile.h"
#include "P_Bouncing.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Bouncing : public AProjectile
{
	GENERATED_BODY()

	AP_Bouncing(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		int32 NumberOfRebounds;
	
};
