// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HiveGravityZone.generated.h"

UCLASS()
class SHOOTERGAME_API AHiveGravityZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHiveGravityZone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityOrientation")
		FRotator Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityOrientation")
		FRotator Right;

// 	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gravity")
// 		FRotator* LookAtRight;
// 	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Gravity")
// 		FRotator* LookAtLeft;


	
};
