// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "DB_Minion.h"

UDB_Minion::UDB_Minion(){
}

void UDB_Minion::Initialize(AActor* ActorToApplybehavior){
	MyMinion = Cast<AMinionCharacter>(ActorToApplybehavior);
	//if (!MyMinion) {
	//	MyMinion = Cast<ASpeeder>(ActorToApplybehavior);
	//}
}


void UDB_Minion::ApplyLifeDamage(float Damage, TSubclassOf<UDamageType> DamageType) {

    MyMinion->Health = FMath::Clamp(MyMinion->Health - Damage, 0.0f, MyMinion->MaxHealt);
}

bool UDB_Minion::IsPawnDied(){
	return (MyMinion->Health <= 0);
}

void UDB_Minion::Killed(AConceptGameMode* MyGameMode, AController* KillerPlayer, AController* KilledPlayer, FAbilityData AbilityData){

	MyMinion->Death();
}
