// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "A_Dome.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_Dome : public AAbility
{
	GENERATED_BODY()
	
public:

	AA_Dome();

	virtual void UseAbility() override;
    virtual void StartAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dome")
		bool bSpawnRestrictToFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dome")
		TSubclassOf<class AActor> DomeProtection;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dome")
		TSubclassOf<class AActor> DomeDamager;



private:

	UFUNCTION(Server, Reliable, withvalidation, blueprintcallable, Category = "Dome" )
		void CreateDome(FVector SpawnPoint, FRotator SpawnRotation);

	UFUNCTION()
		virtual bool IsAbleToFire() override;
};
