// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "MinionController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MinionPathFollowingComponent.h"



AMinionController::AMinionController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMinionPathFollowingComponent>(TEXT("PathFollowingComponent"))) {

}

void AMinionController::KillingMinion(){
	//TODO only call this on server
	if (/*this && */HasAuthority()){
		BrainComponent->StopLogic("die");
	}
}
