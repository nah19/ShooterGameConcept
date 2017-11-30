// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "A_TurretShoot.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_TurretShoot : public APawn
{
	GENERATED_BODY()


	//UDestructibleComponent* DestructibleTurret;
	USceneComponent *Spawner;
	USceneComponent *SpawnerPoint;
	UArrowComponent *Arrow;

	template <typename T>
	T* GetSpecificComponent(TArray<T*> &Components, FString ComponentName);

	TSubclassOf<class AProjectile> MainProjectileClass;
	bool IsFiring = false;
	FTimerHandle Handle;

	
	//void ShootProjectile();
	//void TurretMainUse();
	float DistanceToTarget();
	bool TargetIsValid();
	FVector TargetLocation();

	UFUNCTION(NetMulticast, reliable, withvalidation)
	void AimTo(FVector TargetLocation, float DeltaTime);

	//UFUNCTION(Server, reliable, withvalidation)
	//	void ServerSpawnProjectile(FVector StartLocation, FRotator StartRotation, TSubclassOf<class AProjectile> ProjectileClass);


public:
	AA_TurretShoot(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		AActor *Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TurretTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FiringRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TurretRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TeamNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool ManualControl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystemComponent* Beam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* BeamParticle;
	void Explode();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* ExplosionParticle;

protected:

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	UFUNCTION(NetMulticast, Reliable)
		void Death();
	UFUNCTION(NetMulticast, Reliable)
		void DelayedDestroy();

};
