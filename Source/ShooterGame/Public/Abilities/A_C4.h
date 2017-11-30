// Fill out your copyright notice in the Description page of Project Settings.

#include "A_BaseWeapon.h"
#include "Projectiles/P_C4.h"
#include "A_C4.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_C4 : public AA_BaseWeapon
{
	GENERATED_BODY()
	
public:

	AA_C4();

	UFUNCTION()
		virtual void PerformSpecialBehavour() override;

	UFUNCTION()
		virtual void UseAbility() override;



private:

	bool bChargeC4 = false;
	bool bThrowC4 = false;

	float Increment = 1.0;
	float CurrentForce = 0.0f;
	float FinalForce = 0.0f;

	void ChargeC4(float DeltaSeconds);

	void ThrowC4();
};
