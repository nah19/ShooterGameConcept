// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_Dome.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Dome : public AProjectile
{
	GENERATED_BODY()
	
public:
	AP_Dome(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;

	virtual void OnHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
