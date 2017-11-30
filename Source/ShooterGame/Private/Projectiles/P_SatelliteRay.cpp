// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_AssaultWeapon.h"
#include "P_SatelliteRay.h"


AP_SatelliteRay::AP_SatelliteRay(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
	//TODO: cadencia y municion

}

void AP_SatelliteRay::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	UParticleSystemComponent* Particle  = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, this->GetActorLocation());
	Particle->SetWorldScale3D(FVector(SizeOfParticle, SizeOfParticle, SizeOfParticle));

	APawn* Character = Cast<APawn>(GetOwner());
	if (Character){
		UGameplayStatics::ApplyRadialDamage(this, 90.f, GetActorLocation(), 700.f, DamageType, TArray<AActor*>(), this, (AController*)Character->Controller);
	}
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_SatelliteRay::Explode);
	GetWorldTimerManager().SetTimer(Handle, TimerDelegate, 6.0f, false);
}
void AP_SatelliteRay::Explode() {
	Destroy();
}

