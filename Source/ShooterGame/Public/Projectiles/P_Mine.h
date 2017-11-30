// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_Mine.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Mine : public AProjectile
{
	GENERATED_BODY()
	
public:

	AP_Mine(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* SonidoExplosionCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
		float Life = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
		float RadiusEffect = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Mine")
		bool bActivate = false;

	UFUNCTION()
		virtual void EffectVisual(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	UFUNCTION()
		virtual void EffectLogic(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	UFUNCTION()
		void Explode();
	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* MyComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
