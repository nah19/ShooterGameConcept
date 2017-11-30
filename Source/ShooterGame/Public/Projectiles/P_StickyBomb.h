// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "P_StickyBomb.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_StickyBomb : public AProjectile
{
	GENERATED_BODY()
	
public:

	AP_StickyBomb(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	UFUNCTION(NetMulticast, Reliable, Category = "sticky")
		void Explode(const FHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation, Category = "sticky")
		void ExplodeServer(const FHitResult Hit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* SonidoExplosionCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* HitParticle;

};
