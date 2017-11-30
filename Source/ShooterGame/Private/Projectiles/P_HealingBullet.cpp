// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "DamageTypes/D_AssaultWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "P_HealingBullet.h"


AP_HealingBullet::AP_HealingBullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAffectTeammates = true;

	HealingFactor = -UKismetMathLibrary::Abs(HealingFactor);
	DamageBase = HealingFactor;
}

void AP_HealingBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP_HealingBullet::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, UD_AssaultWeapon::StaticClass());
	}
	Destroy();
}

