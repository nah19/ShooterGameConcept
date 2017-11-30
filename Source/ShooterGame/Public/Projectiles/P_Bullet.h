// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "P_Bullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Bullet : public AProjectile
{
	GENERATED_BODY()
	
public:
	AP_Bullet(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;

};
