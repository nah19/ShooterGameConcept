// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "ConceptSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AConceptSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	AConceptSpectatorPawn( const FObjectInitializer& ObjectInitializer );

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

};
