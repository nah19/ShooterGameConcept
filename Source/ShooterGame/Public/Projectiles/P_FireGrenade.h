// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/Projectile.h"
#include "P_FireGrenade.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_FireGrenade : public AProjectile
{
	GENERATED_BODY()
	
public:

	AP_FireGrenade(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;


	UFUNCTION(BlueprintImplementableEvent, Category = "FireGrenade")
		void SpawnFire();
	UFUNCTION(BlueprintImplementableEvent, Category = "FireGrenade")
		void SpawnGrenade();
	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	UFUNCTION(Server, WithValidation, unreliable)
		void Explode();
	UFUNCTION(client, unreliable)
		void ExplodeEmitter();
	UFUNCTION(Server, WithValidation, unreliable)
		void DestroyBomb();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* SonidoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireGrenade")
		TSubclassOf<AProjectile> Fire;
	UPROPERTY(BlueprintReadOnly, Category = "FireGrenade")
		FHitResult HitSurface;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireGrenade")
		bool bIsGrenade;

private:
	bool isDestroyed;
};
