#pragma once

#include "GameFramework/Actor.h"
#include "ConceptDamageType.h"
#include "Ability.generated.h"


class AConceptCharacter;
class AProjectile;




namespace AbilitySocket
{
	const FName Weapon0;
	const FName Weapon1;
	const FName Weapon2;
	const FName Weapon3;
	const FName Weapon4;

}

UCLASS(Blueprintable)
class SHOOTERGAME_API AAbility : public AActor {
	GENERATED_BODY()

public:

	AAbility();

	//CUSTOMIZABLE PROPERTIES

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Ability")
		FAbilityData AbilityData;

	UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "Ability")
		FText NameOfAbility;

	UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "Ability")
		FText DescriptionOfAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<class UConceptDamageType> DamageTypeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseWeapon")
		float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int32 ClipSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float Cadency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int32 TotalAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float DamageBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		bool bUniqueSimpleAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		bool bAffectsShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		bool bHaveSpecialBehaviour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		FName SocketToUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		bool OrientedToCursor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		UAnimMontage* ChangeAbilityMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		UAnimMontage* AbilityMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		UAnimSequence* IdleAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float AbilityMontageRate = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<class AProjectile> ProjectileClass;



	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ability")
		float TimeRemainingToFire;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ability")
		float BaseTimeToFire;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ability")
		int32 CurrentAmmo;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ability")
		EStateOfAbility StateOfAbility;



	UPROPERTY()
		FName CurrentSocketUsed;
	UPROPERTY()
		bool IsFiring;


	UFUNCTION()
	void FireAbility();

	UFUNCTION()
	virtual void StartAbility();

	UFUNCTION()
	virtual void StopAbility();

	UFUNCTION(blueprintcallable, BlueprintNativeEvent, Category = "Ability")
	void StopAbilityEvent();

	UFUNCTION()
		virtual void PerformSpecialBehavour();

	UFUNCTION(blueprintcallable, Category = "Ability")
		FVector_NetQuantize10 GetFirePoint(USkeletalMeshComponent* SkeletalMeshParam = NULL);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		FVector_NetQuantize10 GetPointToCalculateRotationToCursor(USkeletalMeshComponent* SkeletalMeshParam = NULL);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void CheckIfIsFiring();

	UFUNCTION(blueprintcallable, BlueprintNativeEvent, Category = "Ability")
		void OnAnimationEnd();

	UFUNCTION(blueprintcallable, Category = "Ability")
		virtual void UseAbility();

	UFUNCTION(blueprintcallable, BlueprintNativeEvent, Category = "Ability")
		void UseAbilityOverridableLogic();

	UFUNCTION()
		virtual bool IsAbleToFire();

	UFUNCTION(blueprintcallable, BlueprintNativeEvent, Category = "Ability")
		 void StartInUseAbility();

	UFUNCTION(blueprintcallable, BlueprintNativeEvent, Category = "Ability")
		 void StopInUseAbility();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Montage_Play_Multicast(UAnimMontage* Montage, float RelativeTime, bool FillTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
		void OnProjectileSpawned();




protected:
	AProjectile* LastProjectile;

	FTimerHandle AbilityTimers;

	FTimerHandle MainTimerHandle;

	FTimerHandle CanalizeHandle;
	FTimerDelegate CanalizeDelegate;

	TArray<AProjectile*> PrimaryAbilityProjectiles;

	UPROPERTY()
		USceneComponent* RootScene;

	UPROPERTY()
		USceneComponent* RootScene2;

	UPROPERTY()
		UArrowComponent* CenterOfAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		UParticleSystemComponent* ParticleComponent;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		USoundCue* SoundOnFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		FName FirePointSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		FVector_NetQuantize10 FirePointRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		ETypeOfFirePointEnum TypeOfFirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		UParticleSystem* ParticleMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")	//NOT USED
		int32 AnimationIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float Expense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float VelocityMultiplierOnUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		UAnimMontage* ReloadingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		bool bAutoAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float SizeOfMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		bool ShouldCanalize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float CanalizationTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		int32 IndexOfWithWeaponAnim;

	UPROPERTY()
		float TimeSinceLastShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		bool bIsParticleActivated;



	UFUNCTION(BlueprintCallable, Category = "Ability")
		void InitAbility();

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
		void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void BeginPlay() override;

	UFUNCTION()
		void DeductAmmo();

	UFUNCTION()
		AProjectile* SpawnProjectile(FRotator Rotator);

	UFUNCTION()
		AActor* FindControllerByID(int32 ActorId);

	UFUNCTION()
		void TimeRemainingManager(float DeltaSeconds);

	UFUNCTION()
		void UpdateProjectilesArrays();

	UFUNCTION()
		void AddProjectileToArray(AProjectile* Projectile);

	UFUNCTION(Server, reliable, withvalidation)
		void ServerSpawnProjectile(FRotator Rotator);

	UFUNCTION(NetMulticast, WithValidation, Unreliable)
		void SpawnParticle(bool Attached, EAttachLocation::Type AttachMode = EAttachLocation::KeepRelativeOffset);

	UFUNCTION(Server, WithValidation, Unreliable)
		void ServerSpawnParticle(bool Attached, EAttachLocation::Type AttachMode = EAttachLocation::KeepRelativeOffset);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetActivatedParticle(bool Activated);

	UFUNCTION(Server, Reliable, WithValidation)
		void Montage_Play_Server(UAnimMontage* Montage, float RelativeTime, bool FillTime);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void ActivateParticle(bool Activated);

	UFUNCTION(Client, Unreliable)
		void SyncronizingAimRotation(FRotator StartRotation);

	UFUNCTION(Server, Reliable, WithValidation)
		void AbilityMontage_Play_Server();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void AbilityMontage_Play_Multicast();

	UFUNCTION(BlueprintCallable, Category = "ConceptCharacter")
		void PlaySound();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void PlaySound_Server();
	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void PlaySound_Client();
	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void PlaySound_Multicast();

	bool CapsuleTraceMulti(UObject* WorldContextObject, const FVector_NetQuantize10 Start, const FVector_NetQuantize10 End, float Radius, float HalfHeight, ECollisionChannel TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& OutHits, bool bIgnoreSelf);



};


