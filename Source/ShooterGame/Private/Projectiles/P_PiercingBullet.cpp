// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_AssaultWeapon.h"
#include "P_PiercingBullet.h"

AP_PiercingBullet::AP_PiercingBullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

}

void AP_PiercingBullet::BeginPlay(){
	Super::BeginPlay();

	SmokeTrailComponent = UGameplayStatics::SpawnEmitterAttached(SmokeTrail, RootComponent);
	SmokeTrailComponent->SetRelativeScale3D(SmokeTrailComponent->GetComponentScale() + FVector(-0.7, -0.7, -0.7));
}

void AP_PiercingBullet::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	APawn* CharacterToAffect = Cast<APawn>(ActorToAffectParam);

	if (Character && !CharactersHitted.Contains(Character)) {
		CharactersHitted.Add(Character);
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
	}

}
