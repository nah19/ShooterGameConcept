// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_TakeLifePoints.h"
#include "P_StealLifeBullet.h"




AP_StealLifeBullet::AP_StealLifeBullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AP_StealLifeBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP_StealLifeBullet::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
	}
	Destroy();
}


