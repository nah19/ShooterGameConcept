// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_Laser.h"
#include "Ability.h"
#include "D_AssaultWeapon.h"
#include "ConceptCharacter.h"

AP_Laser::AP_Laser(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AP_Laser::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character){
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
	}
	else{
		SpawnNormalImpactDecal(Hit);
	}
	Destroy();
}

void AP_Laser::SpawnHitEffect(const FHitResult &Hit){
	if (!OnCharacterHittedEffect(Hit))
	{
		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesOnHit[1], Hit.Location);
		Particle->SetWorldScale3D(FVector(SizeOfParticle, SizeOfParticle, SizeOfParticle));
	}
	else {
		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesOnHit[0], Hit.Location);
		Particle->SetWorldScale3D(FVector(SizeOfParticle, SizeOfParticle, SizeOfParticle));
	}
}
