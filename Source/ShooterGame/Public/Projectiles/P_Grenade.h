// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Projectile.h"
#include "P_Grenade.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Grenade : public AProjectile
{
	GENERATED_BODY()

public:

	AP_Grenade(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* SonidoExplosion;

	virtual void BeginPlay() override;
	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;

	UFUNCTION(Server, WithValidation, unreliable)
		void Explode();
	UFUNCTION(client, unreliable)
		void ExplodeEmitter();
	UFUNCTION()
		virtual void EffectVisual(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	UFUNCTION()
		virtual void EffectLogic(AActor* ActorToAffectParam, const FHitResult& Hit) override;


};
