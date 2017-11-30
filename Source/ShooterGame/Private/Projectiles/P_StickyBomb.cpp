// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_StickyBomb.h"
#include "D_AssaultWeapon.h"
#include "ConceptCharacter.h"

AP_StickyBomb::AP_StickyBomb(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//static ConstructorHelpers::FObjectFinder<USoundCue> Sonido(TEXT("/Game/Audio/Explosion_Cue.Explosion_Cue"));
	//SonidoExplosionCue = Sonido.Object;
}


void AP_StickyBomb::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	AConceptCharacter* CharacterHitted = Cast<AConceptCharacter>(ActorToAffectParam);
	AConceptCharacter* SelfCharacter = Cast<AConceptCharacter>(GetOwner());
	if (CharacterHitted && SelfCharacter && SelfCharacter == CharacterHitted) {
		return;
	}
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_StickyBomb::ExplodeServer, Hit);
	GetWorldTimerManager().SetTimer(Handle, TimerDelegate, 3.0f, false);

	if (CharacterHitted) {
		FName BoneHitted = CharacterHitted->GetMesh()->FindClosestBone(Hit.Location);
		UStaticFunctionLibrary::PrintString(this, BoneHitted.ToString());
		this->AttachToActor(ActorToAffectParam, FAttachmentTransformRules::KeepWorldTransform, BoneHitted);
	}
}

void AP_StickyBomb::ExplodeServer_Implementation(const FHitResult Hit) {
	Explode(Hit);
}
bool AP_StickyBomb::ExplodeServer_Validate(const FHitResult Hit) {
	return true;
}

void AP_StickyBomb::Explode_Implementation(const FHitResult Hit) {
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SonidoExplosionCue, GetActorLocation(), 2);
	SpawnExplosionEffect();
	SpawnExplosionDecal(Hit);

	APawn* Character = Cast<APawn>(GetOwner());
	if (Character){
		UGameplayStatics::ApplyRadialDamage(this, DamageBase, GetActorLocation(), EffectArea, DamageType, TArray<AActor*>(), this, (AController*)Character->Controller);
	}
	
	Destroy();
}
