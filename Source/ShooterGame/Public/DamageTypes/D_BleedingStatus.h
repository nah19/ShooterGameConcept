// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "DamageTypes/ConceptDamageType.h"
#include "D_BleedingStatus.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UD_BleedingStatus : public UConceptDamageType
{
	GENERATED_BODY()
	
	class UClass* CamShake;
	TSubclassOf<class UCameraShake> CameraShakeClass;
	
public:
	UD_BleedingStatus();
	virtual void DamagePlayer(float Damage, AController* KillerPlayer, AController* KilledPlayer, UConceptDamageBehavior* DamageBehavior);

};
