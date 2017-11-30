// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_Bullet.h"
#include "Ability.h"
#include "ConceptCharacter.h"
#include "D_AssaultWeapon.h"
#include "StaticFunctionLibrary.h"


AP_Bullet::AP_Bullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void AP_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP_Bullet::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	APawn* CharacterToAffect = Cast<APawn>(ActorToAffectParam);
	
	if (Character){UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);}
	
	Destroy();
}