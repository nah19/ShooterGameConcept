// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_Arrow.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Arrow : public AProjectile
{
	GENERATED_BODY()
	
public:

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;


	UPROPERTY()
		USoundCue* SonidoExplosionCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* HitParticle;

	UPROPERTY(BlueprintReadWrite, Category = "Arrow")
		bool bSpeedSetted = false;

	UPROPERTY(BlueprintReadOnly, Category = "Arrow")
		float InitialSpeed;

	 
	UFUNCTION(Server, WithValidation, Reliable)
		void SetInitialSpeed(float Speed);

	UFUNCTION(NetMulticast, Reliable)
		void SetInitialSpeed_Clients(float Speed);



private:

	void SetPhysicsSpeed(float Speed);

};
