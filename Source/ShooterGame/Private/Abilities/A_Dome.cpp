// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "A_Dome.h"


AA_Dome::AA_Dome() {
}   

void AA_Dome::StartAbility(){
    if(IsAbleToFire()){
        Super::StartAbility();
    }
}

void AA_Dome::UseAbility(){
	Super::UseAbility();

	AConceptCharacter* OwnerCharacter = Cast<AConceptCharacter>(GetOwner());
	if (OwnerCharacter) {
		FVector SpawnDomeLocation = OwnerCharacter->GetActorLocation();
		UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
		SpawnDomeLocation = MovementComp->CurrentFloor.HitResult.ImpactPoint;
        FRotator SpawnDomeRotation = MovementComp->CurrentFloor.HitResult.ImpactNormal.Rotation();
        //CreateDome(SpawnDomeLocation, SpawnDomeRotation);
	}
}

void AA_Dome::CreateDome_Implementation(FVector SpawnPoint, FRotator SpawnRotation) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	AActor* Dome = GetWorld()->SpawnActor<AActor>(DomeProtection, SpawnPoint, SpawnRotation, SpawnParams);
	if (Dome) {
		AProjectile* DomeProjectile = Cast <AProjectile>(Dome);
		if (DomeProjectile) {
			DomeProjectile->AbilityData = AbilityData;
		}
	}
}

bool AA_Dome::CreateDome_Validate( FVector SpawnPoint, FRotator SpawnRotation) {
	return true;
}

bool AA_Dome::IsAbleToFire() {


	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	return 	!MyCharacter->GetMovementComponent()->IsFalling();


	return true;
}
