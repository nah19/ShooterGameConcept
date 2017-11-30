// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "P_Laser.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Laser : public AProjectile
{
	GENERATED_BODY()

public:
	AP_Laser(const FObjectInitializer& ObjectInitializer);
		virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	virtual void SpawnHitEffect(const FHitResult &Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* HitParticle;
};
