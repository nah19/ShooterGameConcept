// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "A_GravityHammer.h"


AA_GravityHammer::AA_GravityHammer()
	:Super()
{
	SkeletalMeshComponent->SetVisibility(false);
}

void AA_GravityHammer::StartAbility() {
	Super::StartAbility();

}

bool AA_GravityHammer::IsAbleToFire() {

	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	return 	!MyCharacter->GetMovementComponent()->IsFalling();;
	
	//return true;
}
