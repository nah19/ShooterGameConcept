// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_Dome.h"

AP_Dome::AP_Dome(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AP_Dome::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	//TODO: When hit ground, open dome
}

void AP_Dome::OnHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	 
}




