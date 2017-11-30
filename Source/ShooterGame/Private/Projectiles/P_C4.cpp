// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_AssaultWeapon.h"
#include "P_C4.h"


void AP_C4::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_C4::Explode);

	GetWorldTimerManager().SetTimer(Handle, TimerDelegate, 20, false);
}

void AP_C4::Explode() {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SonidoExplosionCue, GetActorLocation(), 2);
		SpawnExplosionEffect();
		SpawnExplosionDecal(FirstHit);
		UGameplayStatics::ApplyRadialDamage(this, DamageBase, GetActorLocation(), EffectArea, UD_AssaultWeapon::StaticClass(), TArray<AActor*>(), this, (AController*)Character->Controller);
	}
	Destroy();
}


void AP_C4::Activate_Implementation(){
	Activate_Clients();
	Explode();
}

bool AP_C4::Activate_Validate(){return true;}

void AP_C4::Activate_Clients_Implementation(){Explode();}