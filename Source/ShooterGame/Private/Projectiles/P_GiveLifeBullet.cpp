// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_GiveLifePoints.h"
#include "P_GiveLifeBullet.h"



AP_GiveLifeBullet::AP_GiveLifeBullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AP_GiveLifeBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP_GiveLifeBullet::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
	}
	Destroy();
}

void AP_GiveLifeBullet::AffectTeammates(bool bAffect)
{
	bAffectTeammates = bAffect;
}


