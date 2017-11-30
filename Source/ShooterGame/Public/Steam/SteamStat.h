// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SteamStat.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API USteamStat : public UObject
{
	GENERATED_BODY()

public:

	USteamStat();

	void InitStat(FString Name);

	UFUNCTION()
		FString GetStatInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamStat")
		FString StatName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamStat")
		float StatValue;

	
	
};
