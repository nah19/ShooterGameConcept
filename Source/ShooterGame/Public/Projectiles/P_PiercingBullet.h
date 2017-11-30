// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_PiercingBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_PiercingBullet : public AProjectile
{
	GENERATED_BODY()
	
	AP_PiercingBullet(const FObjectInitializer& ObjectInitializer);

public:
	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* SmokeTrail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystemComponent* SmokeTrailComponent;
	
private:
	TArray<APawn*> CharactersHitted;
};
