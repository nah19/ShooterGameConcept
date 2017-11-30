// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Abilities/A_BaseWeapon.h"
#include "A_HealinLink.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_HealinLink : public AA_BaseWeapon
{
	GENERATED_BODY()

public:

		AA_HealinLink();

		UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "heal")
		bool bShouldHeal;

	virtual void UseAbility() override;

	virtual void StopAbility() override;

private:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(server, withvalidation, reliable)
	void ApplyDamage(AConceptCharacter* OwnerCharacter);

	AConceptCharacter* HealTarget;
};
