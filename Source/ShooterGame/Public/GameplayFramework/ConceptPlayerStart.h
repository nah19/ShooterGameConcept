// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "ConceptCharacter.h"
#include "ConceptPlayerState.h"
#include "ConceptPlayerStart.generated.h"


UCLASS()
class SHOOTERGAME_API AConceptPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	AConceptPlayerStart( const FObjectInitializer& ObjectInitializer );
	
	public:
	/** Which team can start at this point */
	UPROPERTY( EditInstanceOnly, Category = Team )
	int32 SpawnTeam;

	/** Whether players can start at this point */
	UPROPERTY( EditInstanceOnly, Category = Team )
		uint32 bNotForPlayers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStart")
	UClass* ClassToSpawn;

	UPROPERTY()
		AConceptPlayerState* StateOfLastPlayerSpawned;

	UFUNCTION()
		bool CheckIfBusy(  );

	UFUNCTION()
		void RemovePlayerStateLink();

	UFUNCTION()
	void SetPlayerStateLink( AConceptPlayerState* PlayerStateParam );
	
};
