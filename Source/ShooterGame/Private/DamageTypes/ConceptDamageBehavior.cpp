// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptGameMode.h"
#include "ConceptDamageBehavior.h"

UConceptDamageBehavior::UConceptDamageBehavior(){


}


void UConceptDamageBehavior::Initialize(AActor* ActorToApplybehavior){


}

void UConceptDamageBehavior::ApplyLifeDamage(float Damage, TSubclassOf<UDamageType> DamageType) {

}

void UConceptDamageBehavior::ApplyHurtShout(float Damage){

}

void UConceptDamageBehavior::ApplyLifeRecovery(float Recover)
{

}

void UConceptDamageBehavior::ApplyRadialImpulse(AController* KillerPlayer, FVector Location, float Radius, float Damage, float TimeToStop) {

}

void UConceptDamageBehavior::ApplyHitAnim(float Time, float Damage) {

}

void UConceptDamageBehavior::ApplyPoisonDamage(AController* KillerPlayer, float Damage, float poisonStatusTimer, float poisonDamageTimer) {

}

void UConceptDamageBehavior::ApplyConstrictionDamage(AController* KillerPlayer, float Damage, float constrictionStatusTimeLeft, float constrictionDamageTimeLeft) {

}


void UConceptDamageBehavior::ApplyBurnDamage(AController* KillerPlayer, float Damage) {

}

void UConceptDamageBehavior::ApplyDisruptDamage(float DisruptTime) {
    
}

void UConceptDamageBehavior::ApplyStunDamage(float StunTime) {
    
}

void UConceptDamageBehavior::ApplySnareDamage(float SnareTime) {
    
}

void UConceptDamageBehavior::ApplySlowDamage(float SlowTime, float SlowAmount) {
    
}

void UConceptDamageBehavior::ZeroGravityStatus(float Time)
{

}

bool UConceptDamageBehavior::IsPawnDied(){
	return true;
}

void UConceptDamageBehavior::Killed(AConceptGameMode* MyGameMode, AController* KillerPlayer, AController* KilledPlayer, FAbilityData AbilityData){

}
