// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_FireGrenade.h"
#include "Ability.h"
#include "D_AssaultWeapon.h"
#include "ConceptCharacter.h"

AP_FireGrenade::AP_FireGrenade(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	//static ConstructorHelpers::FObjectFinder<USoundCue> Sonido(TEXT("/Game/Audio/Explosion_Cue.Explosion_Cue"));
	//SonidoExplosion = Sonido.Object;

	isDestroyed = false;

}

void AP_FireGrenade::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_FireGrenade::Explode);
	GetWorldTimerManager().SetTimer(Handle, TimerDelegate, 3.0f, false);
}

void AP_FireGrenade::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	if (isDestroyed){return;}

	HitSurface = Hit;
	SpawnExplosionEffect();
	SpawnExplosionDecal(Hit);

	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		if (bIsGrenade)
		{
			SpawnGrenade();
		}
		else
		{
			SpawnFire();
		}
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SonidoExplosion, GetActorLocation(), 2.0);
	if (!isDestroyed)
	{
		Destroy();
		isDestroyed = true;
	}
}

void AP_FireGrenade::Explode_Implementation() {ExplodeEmitter();}

void AP_FireGrenade::ExplodeEmitter_Implementation() {

	FHitResult Hit;
	this->ProjectileEffect(NULL, Hit);

}
bool AP_FireGrenade::Explode_Validate() {return true;}

void AP_FireGrenade::DestroyBomb_Implementation() {Destroy();}

bool AP_FireGrenade::DestroyBomb_Validate() {return true;}
