// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/A_BaseWeapon.h"
#include "A_Bow.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_Bow : public AA_BaseWeapon
{
	GENERATED_BODY()
	
public:

	AA_Bow();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		virtual void StartAbility() override;

	UFUNCTION()
		virtual void StopAbility() override;

	//UFUNCTION(BlueprintCallable, Category = "Bow")
		virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow Properties")
		float MinThrowForce = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow Properties")
		float MaxThrowForce = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow Properties")
		float ForceChargeSpeed = 50.0f;

private:

	bool bChargeArrow = false;
	bool bShootArrow = false;

	bool bSetSpeed = false;

	float Increment = 1.0;
	float CurrentForce = 0.0f;
	float FinalForce = 0.0f;

	void ChargeArrow(float DeltaSeconds);

	void ThrowArrow();

	
};
