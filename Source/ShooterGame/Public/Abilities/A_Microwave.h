// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/A_BaseWeapon.h"
#include "Characters/ConceptCharacter.h"
#include "A_Microwave.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AA_Microwave : public AA_BaseWeapon
{
	GENERATED_BODY()
	
public:

	AA_Microwave();

//	UFUNCTION()
//		virtual void StopAbility() override;
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "Microwave")
//		void Clear();
//
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Microwave")
//		TArray<AConceptCharacter*> CharactersHit;
//
//	UFUNCTION(Server, WithValidation, Reliable)
//		virtual void DamageObjectsHitted(bool IsSomethingHitted, const TArray<FHitResult>& OutHits) override;
//
//	UFUNCTION(BlueprintCallable, Category = "Microwave")
//		void PlayerCanShoot(AConceptCharacter* Player, bool bCanShoot);
//	
//private:
//	bool bClearPlayers = false;
//
//	void ClearPlayers();
//
//	UFUNCTION(Server, WithValidation, Reliable)
//		void SendClear();
//	UFUNCTION(NetMulticast, Reliable)
//		void SendClear_Clients();
};
