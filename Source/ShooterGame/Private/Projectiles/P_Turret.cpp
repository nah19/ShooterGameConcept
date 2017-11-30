// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_AssaultWeapon.h"
#include "P_Turret.h"


AP_Turret::AP_Turret(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	
}
void AP_Turret::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit){
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character){
		UGameplayStatics::ApplyDamage(ActorToAffectParam, 10.f, (AController*)Character->Controller, this, DamageType);
	}
	this->Destroy();
}
