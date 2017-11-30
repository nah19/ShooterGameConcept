// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "HiveGravityZone.h"


// Sets default values
AHiveGravityZone::AHiveGravityZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void AHiveGravityZone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHiveGravityZone::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AHiveGravityZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}