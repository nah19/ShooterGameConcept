// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptGameSession.h"



AConceptGameSession::AConceptGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	HasStarted = false;
}

void AConceptGameSession::StartSession(){
	HasStarted = true;
}

FName AConceptGameSession::GetNameSession(){
	return SessionName;
}