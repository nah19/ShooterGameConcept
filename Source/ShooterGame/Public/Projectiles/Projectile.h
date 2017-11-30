#pragma once

#include "GameFramework/Actor.h"

#include "Ability.h"
#include "ConceptDamageType.h"
#include "Projectile.generated.h"


class AAbility;

UCLASS()
class SHOOTERGAME_API AProjectile : public AActor {
	GENERATED_BODY()

public:
	AProjectile(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere, Category = "Projectile" )
		USphereComponent* CollisionComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Projectile")
		UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Projectile")
		FAbilityData AbilityData;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<UConceptDamageType> DamageType;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Projectile")
		TArray<UMaterialInterface*> BloodDecalMaterials;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
		UMaterialInterface* ImpactDecal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		TArray<UParticleSystem*> ParticlesOnHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float SizeOfParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float SizeOfDecal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		UParticleSystem* ParticleTrail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		bool bAffectsShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		USoundCue* SoundOnHit;
	UPROPERTY()
		FHitResult FirstHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float EffectArea;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Projectile")
		float DamageBase;
	bool bAffectTeammates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		UParticleSystemComponent* ProjectileTrail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* ExplosionParticle;





	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* MyComp, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		virtual void EffectLogic( AActor* ActorToAffectParam, const FHitResult& Hit);
	UFUNCTION()
		virtual void EffectVisual(AActor* ActorToAffectParam, const FHitResult& Hit);
	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable, Category = "Projectile")
		void EffectVisual_Server(AActor* ActorToAffectParam, const FHitResult& Hit);
	UFUNCTION(NetMulticast, Unreliable, WithValidation, BlueprintCallable, Category = "Projectile")
		void EffectVisual_Multicast(AActor* ActorToAffectParam, const FHitResult& Hit);

	UFUNCTION()
		virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit);
	UFUNCTION()
		virtual void SpawnHitEffect(const FHitResult &Hit);


	UFUNCTION()
		bool OnCharacterHittedEffect(const FHitResult &Hit);
	UFUNCTION()
		void SpawnExplosionEffect();
	UFUNCTION()
		void SpawnNormalImpactDecal(const FHitResult &Hit);
	UFUNCTION()
		void SpawnExplosionDecal(const FHitResult &Hit);



	UFUNCTION(BlueprintCallable, Category = "Projectile")
		void PlaySound();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Projectile")
		void PlaySound_Server();
	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Projectile")
		void PlaySound_Client();
	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "Projectile")
		void PlaySound_Multicast();


	UFUNCTION()
		bool IsValidEnemy(AActor *OtherActor);
	UFUNCTION()
		bool RemoveFromArray();



private:
	bool bRemove;
	FVector TrailLocation;
};
