// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Engine.h"
#include "ConceptPlayerStart.h"
#include "ConceptSpectatorPawn.h"

AConceptSpectatorPawn::AConceptSpectatorPawn( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer ) {
}

void AConceptSpectatorPawn::SetupPlayerInputComponent( UInputComponent* InputComponent ) {
	InputComponent->BindAxis("MoveRight", this, &ADefaultPawn::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &ADefaultPawn::MoveUp_World);

	//GetMovementComponent()->StopMovementImmediately();
}

