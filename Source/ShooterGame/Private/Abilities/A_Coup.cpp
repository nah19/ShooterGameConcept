// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "A_Coup.h"


AA_Coup::AA_Coup() {
	//TODO: cadencia y municion

	//forcePush = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForcePush"));
	//forcePush->Radius = 700;
	//forcePush->Falloff = ERadialImpulseFalloff::RIF_Linear;
	//forcePush->ImpulseStrength = 99999.0;
	////forcePush->ForceStrength = 1000000000.0;
	////forcePush->AddCollisionChannelToAffect(ECC_PhysicsBody);
	//forcePush->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	//forcePush->AddObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
}

void AA_Coup::UseAbility(){
	Super::UseAbility();

	PlaySound();

	UParticleSystemComponent* force = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ForceParticle, GetOwner()->GetActorLocation(), FRotator(0,90,0), true);

	force->SetRelativeScale3D(FVector(3, 3, 3));

	forcePush->FireImpulse();
}

