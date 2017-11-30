// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_PoisonBullet.h"
#include "ConceptCharacter.h"
#include "D_PoisonStatus.h"
#include "StaticFunctionLibrary.h"


AP_PoisonBullet::AP_PoisonBullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void AP_PoisonBullet::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	APawn* CharacterToAffect = Cast<APawn>(ActorToAffectParam);

	if (Character) {
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
	}

	Destroy();
}


