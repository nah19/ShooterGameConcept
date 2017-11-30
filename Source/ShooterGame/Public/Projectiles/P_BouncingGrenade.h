// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Projectiles/Projectile.h"
#include "P_BouncingGrenade.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_BouncingGrenade : public AProjectile
{
	GENERATED_BODY()

public:
		AP_BouncingGrenade(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	void GrenadeExplode(const FHitResult& Hit);
	UFUNCTION(Server, WithValidation, reliable)
		void Explode();
	UFUNCTION(NetMulticast, reliable)
		void ExplodeEmitter();
	//UFUNCTION(Server, WithValidation, unreliable)
	//	void DestroyBomb();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* SonidoExplosion;

	
};
