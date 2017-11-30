// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/P_Bullet.h"
#include "P_FireBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_FireBullet : public AP_Bullet
{
	GENERATED_BODY()
	
public:
	AP_FireBullet(const FObjectInitializer& ObjectInitializer);


	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	
};
