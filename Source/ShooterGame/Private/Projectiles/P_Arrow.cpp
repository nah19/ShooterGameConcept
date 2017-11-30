// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_Arrow.h"
#include "D_AssaultWeapon.h"

void AP_Arrow::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
	}
	Destroy();
}

void AP_Arrow::SetInitialSpeed_Implementation(float Speed){
	SetInitialSpeed_Clients(Speed);
	SetPhysicsSpeed(Speed);
}

bool AP_Arrow::SetInitialSpeed_Validate(float Speed){return true;}

void AP_Arrow::SetInitialSpeed_Clients_Implementation(float Speed){	SetPhysicsSpeed(Speed);}

void AP_Arrow::SetPhysicsSpeed(float Speed)
{
	InitialSpeed = ProjectileMovementComponent->InitialSpeed + Speed;

	FVector Velocity = GetVelocity();
	Velocity.Normalize();
	Velocity *= InitialSpeed;

	CollisionComponent->SetPhysicsLinearVelocity(Velocity);

	bSpeedSetted = true;

}
