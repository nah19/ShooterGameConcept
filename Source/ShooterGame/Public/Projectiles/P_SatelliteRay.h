// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_SatelliteRay.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_SatelliteRay : public AProjectile
{
	GENERATED_BODY()

		virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
public:
	AP_SatelliteRay(const FObjectInitializer& ObjectInitializer);
	UFUNCTION()
		void Explode();

};
