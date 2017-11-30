// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "Projectile.h"
#include "A_SatelliteAttack.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_SatelliteAttack : public AAbility
{
	GENERATED_BODY()
	
	AA_SatelliteAttack();

	virtual void UseAbility() override;

	TSubclassOf<class AProjectile> MyItemBlueprint;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "satellite")
		TSubclassOf<class AProjectile> RayClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "satellite")
		TSubclassOf<class AProjectile> SatelliteTrails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "satellite")
		FVector AltenumCenter;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "satellite")
		UDecalComponent* DecalLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "satellite")
		UDecalComponent* DecalCenter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "satellite")
		UDecalComponent* DecalRight;*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
		void OnRaySpawned();


};
