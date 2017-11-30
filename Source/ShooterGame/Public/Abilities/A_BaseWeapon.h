// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "A_BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class ETypeOfWeapon : uint8 {
	RE_NORMAL 	UMETA(DisplayName = "RE_NORMAL"),
	RE_BURST 	UMETA(DisplayName = "RE_BURST"),
	RE_CONTINUOUS 	UMETA(DisplayName = "RE_CONTINUOUS"),
	RE_INSTANTBURST	UMETA(DisplayName = "RE_INSTANTBURST"),
	RE_MELEE	UMETA(DisplayName = "RE_MELEE")
};


/**
 *
 */
UCLASS()
class SHOOTERGAME_API AA_BaseWeapon : public AAbility
{
	GENERATED_BODY()

public:

	AA_BaseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void StartReload();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		int32 Desviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		int32 ShotsPerBurst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		AActor*  HitBox;

	UPROPERTY()
		bool IsJammed;

	UPROPERTY()
		float TimeJammed;

	UPROPERTY()
		int32 CurrentShotInBurst;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float InternalBurstCadency;

	UPROPERTY()
		bool IsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		bool IsFastProjectile;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float ContinuousDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float DistanceOfEffectContinuous;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float RadiusOfEffectContinuous;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		ETypeOfWeapon TypeOfWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		TSubclassOf<AActor> WeaponMeleeHitBox;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProtected")
		float WeaponMeleeStaminaWaste;


	UFUNCTION()
		void BurstShoot();

	UFUNCTION()
		void ContinuousShot();	//???

	UFUNCTION()
		void EndReload();

	UFUNCTION()
		bool EnoughTimeSinceLastShot();

	UFUNCTION()
		bool HasCurrentAmmo();

	UFUNCTION()
		bool HasTotalAmmo();

	UFUNCTION()
		void CheckIfIsFiringContinous();

	UFUNCTION()
		bool HasEnoughStamina();

	UFUNCTION()
		void ReduceStamina();

	UFUNCTION(BlueprintCallable, Category = "AbilityProtected")
		void InitWeapon();

	UFUNCTION(BlueprintNativeEvent, Category = "AbilityProtected")
		void Fire();

	UFUNCTION(Server, WithValidation, Reliable)
		void Shot(FRotator FinalRotation);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void SetParticleRotation_Multicast(FRotator StartRotation);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetParticleRotation_Server(FRotator StartRotation);

	UFUNCTION(Server, WithValidation, Reliable)
		virtual void DamageObjectsHitted(bool IsSomethingHitted, const TArray<FHitResult>& OutHits);

	UFUNCTION(NetMulticast, Reliable)
		void ShotReplicated(FRotator FinalRotation);

	UFUNCTION(Client, WithValidation, Reliable)
		void NormalShot();

	UFUNCTION(Client, WithValidation, Reliable)
		void InstantBurstShoot();

	UFUNCTION(Client, WithValidation, Reliable)
		void ContinuousShotDetector(FRotator RotationToAim, FVector_NetQuantize10 EndPos);	//???

	virtual void UseAbility() override;


};


