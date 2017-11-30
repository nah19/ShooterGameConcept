// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_C4.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_C4 : public AProjectile
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "C4 Explosive")
		void Explode();
	UFUNCTION(Server, WithValidation, Reliable)
		void Activate();
	UFUNCTION(NetMulticast, Reliable)
		void Activate_Clients();

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		USoundCue* SonidoExplosionCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* HitParticle;


	
};
