// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameSession.h"
#include "ConceptGameSession.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AConceptGameSession : public AGameSession
{
	GENERATED_BODY()
	

	AConceptGameSession(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	bool HasStarted;
	
public:
	UFUNCTION()
		void StartSession();

	UFUNCTION(BlueprintCallable, Category = "GameSession")
		FName GetNameSession();
};


