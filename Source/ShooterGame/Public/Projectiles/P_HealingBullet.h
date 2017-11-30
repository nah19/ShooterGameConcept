// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_HealingBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_HealingBullet : public AProjectile
{
	GENERATED_BODY()
	
public:
	AP_HealingBullet(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "HealingBullet")
		float HealingFactor;
};
