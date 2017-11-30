// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_AssaultWeapon.h"
#include "P_Mine.h"

AP_Mine::AP_Mine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//static ConstructorHelpers::FObjectFinder<USoundCue> Sonido(TEXT("/Game/Audio/Explosion_Cue.Explosion_Cue"));
	//SonidoExplosionCue = Sonido.Object;
}



void AP_Mine::OnSphereBeginOverlap(UPrimitiveComponent* MyComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	// Que la mina no explote hasta que esté activada (se activa en el event graph de la blueprint)
	if (bActivate && OtherActor)
	{
		Explode();
	}
}

void AP_Mine::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {

	AConceptCharacter* EnemyPlayer = Cast<AConceptCharacter>(ActorToAffectParam);

	if (bActivate && EnemyPlayer) {
		return Explode();
	}

	FTimerHandle Handle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_Mine::Explode);

	GetWorldTimerManager().SetTimer(Handle, TimerDelegate, Life, false);
}

void AP_Mine::EffectVisual(AActor* ActorToAffectParam, const FHitResult& Hit) {
	SpawnExplosionDecal(Hit);
	SpawnExplosionEffect();
}


void AP_Mine::EffectLogic(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character)
	{
		UGameplayStatics::ApplyRadialDamage(this, DamageBase, GetActorLocation(), EffectArea, DamageType, TArray<AActor*>(), this, (AController*)Character->Controller);
	}
}

void AP_Mine::Explode() {
	EffectVisual(NULL, FirstHit);
	EffectLogic(NULL, FirstHit);

	Destroy();
}

