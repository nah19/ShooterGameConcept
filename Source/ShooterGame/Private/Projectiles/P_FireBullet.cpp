// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_FireBullet.h"
#include "ConceptCharacter.h"
#include "D_BurnStatus.h"
#include "StaticFunctionLibrary.h"



AP_FireBullet::AP_FireBullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void AP_FireBullet::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());

	if (Character) {
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
	}

	Destroy();
}

