// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_StealLifeBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_StealLifeBullet : public AProjectile
{
	GENERATED_BODY()
	

public:
	AP_StealLifeBullet(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	
};
