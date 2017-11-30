// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_Grenade.h"
#include "Ability.h"
#include "D_AssaultWeapon.h"
#include "ConceptCharacter.h"

AP_Grenade::AP_Grenade(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	//static ConstructorHelpers::FObjectFinder<USoundCue> Sonido(TEXT("/Game/Audio/Explosion_Cue.Explosion_Cue"));
	//SonidoExplosion = Sonido.Object;
	
}

void AP_Grenade::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_Grenade::ExplodeEmitter);
	GetWorldTimerManager().SetTimer(Handle, TimerDelegate, 3.0f, false);
}

void AP_Grenade::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	EffectVisual(ActorToAffectParam, Hit);
	EffectLogic(ActorToAffectParam, Hit);
	Destroy();
}

void AP_Grenade::EffectVisual(AActor* ActorToAffectParam, const FHitResult& Hit) {
	SpawnExplosionEffect();
	SpawnExplosionDecal(Hit);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SonidoExplosion, GetActorLocation(), 2.0);
}


void AP_Grenade::EffectLogic(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::ApplyRadialDamage(this, DamageBase, GetActorLocation(), EffectArea, DamageType, TArray<AActor*>(), this, (AController*)Character->Controller, true, ECollisionChannel::ECC_GameTraceChannel5);
	}
}

void AP_Grenade::ExplodeEmitter_Implementation() {
	FHitResult Hit;
	this->ProjectileEffect(NULL, Hit);
}

void AP_Grenade::Explode_Implementation() {
	ExplodeEmitter();
}

bool AP_Grenade::Explode_Validate(){
	return true;
}