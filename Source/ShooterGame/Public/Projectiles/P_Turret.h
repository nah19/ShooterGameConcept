// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "P_Turret.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Turret : public AProjectile
{
	GENERATED_BODY()

public:

	AP_Turret(const FObjectInitializer& ObjectInitializer);
	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	
	
};
