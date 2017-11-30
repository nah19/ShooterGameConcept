// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
 //#include "ConceptCharacter.h"
 //#include "MinionController.h"
#include "DB_Minion.h"
#include "MinionPathFollowingComponent.h"
#include "Speeder.h"
#include "HiveGravityZone.h"
#include "CustomCharacterMovementComponent.h"


ASpeeder::ASpeeder(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
	PrimaryActorTick.bCanEverTick = true;
	DifficultPoints = 1;
	MaxHealt = 20;
	Health = MaxHealt;
	Speed = 50;
	MapPawnOrientations.SetNumUninitialized(2);

	generation = 0;

}


void ASpeeder::NewGeneration() {
	generation+=1;
	Health = MaxHealt;
	UE_LOG(LogTemp, Log, TEXT("Generation: %d"), generation);

}
