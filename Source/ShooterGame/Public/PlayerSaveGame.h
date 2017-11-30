// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPlayerSaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SavedData")
		TSubclassOf<UObject> DefaultCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SavedData")
		TArray<TSubclassOf<UObject>> DefaultAbilities;
	
	
};
