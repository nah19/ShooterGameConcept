// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "P_Misile.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_Misile : public AProjectile
{
	GENERATED_BODY()

public:

	AP_Misile(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	float normalizeAngle(float angle);

	void CalculateMovement(AConceptPlayerController* Controller);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float BulletEffectForce = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		bool bIsRemoteControlled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float MaxAngle = 90;

	UFUNCTION(Server, WithValidation, Reliable)
		void SetPosition(FVector_NetQuantize10 Position);

	UFUNCTION()
		virtual void EffectVisual(AActor* ActorToAffectParam, const FHitResult& Hit) override;

	UFUNCTION()
		virtual void EffectLogic(AActor* ActorToAffectParam, const FHitResult& Hit) override;

	UFUNCTION(NetMulticast, Reliable)
		void SetPosition_Clients(FVector_NetQuantize10 Position);

	UFUNCTION()
		void Explode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* SmokeTrail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystemComponent* SmokeTrailComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Target;


	FVector Position;
	FVector BulletDirection;
	FVector MousePosition;
	FVector Bullet2Mouse;
	FVector NewDirection;

	bool bKeepFollowing = true;
	
};
