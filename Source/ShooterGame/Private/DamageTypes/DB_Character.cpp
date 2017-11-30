// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptGameMode.h"
#include "MinionController.h"
#include "DB_Character.h"


UDB_Character::UDB_Character(){
}


void UDB_Character::Initialize(AActor* ActorToApplybehavior){
	MyCharacter = Cast<AConceptCharacter>(ActorToApplybehavior);
}


void UDB_Character::ApplyLifeDamage(float Damage, TSubclassOf<UDamageType> DamageType) {

	if (!MyCharacter) { return; }


	Damage = Damage * ((100 - MyCharacter->Armor) / 100);

	MyCharacter->Life = FMath::Clamp(MyCharacter->Life - Damage, 0.0f, MyCharacter->LifeMax);




	MyCharacter->MyPlayerController->PlayForceFeedback(Damage);

	if (!MyCharacter->IsRegenerating){
		MyCharacter->LifeRegenerationSystem();
	}
	if (DamageType) {
		MyCharacter->OnLifeDamageReceive(Damage, DamageType);
	}

}

void UDB_Character::ApplyDisruptDamage(float DisruptTime) {
	if (!MyCharacter) { return; }
	MyCharacter->StartDisruptStatusServer(DisruptTime);
}
void UDB_Character::ApplyStunDamage(float StunTime) {
    if (!MyCharacter) { return; }
    MyCharacter->StartStunStatusServer(StunTime);
}
void UDB_Character::ApplySnareDamage(float SnareTime) {
    if (!MyCharacter) { return; }
    MyCharacter->StartSnareStatusServer(SnareTime);
}
void UDB_Character::ApplySlowDamage(float SlowTime, float SlowAmount) {
    if (!MyCharacter) { return; }
    MyCharacter->StartSlowStatusServer(SlowTime, SlowAmount);
}

void UDB_Character::ApplyLifeRecovery(float Recover) {
	
	if (!MyCharacter) { return; }

	MyCharacter->Life = FMath::Clamp(MyCharacter->Life + Recover, MyCharacter->Life, MyCharacter->LifeMax);
}

void UDB_Character::ApplyPoisonDamage(AController* KillerPlayer, float Damage, float poisonStatusTimer, float poisonDamageTimer)
{
	if (!MyCharacter) { return; }

	MyCharacter->SetPoisonStatus(KillerPlayer, Damage, poisonStatusTimer, poisonDamageTimer);
	MyCharacter->MyPlayerController->ClientPlayForceFeedback(DamageForceFeedback, false, "None");
}

void UDB_Character::ApplyBurnDamage(AController* KillerPlayer, float Damage)
{
	if (!MyCharacter) { return; }

	MyCharacter->SetBurnStatus(KillerPlayer, Damage);
	MyCharacter->MyPlayerController->ClientPlayForceFeedback(DamageForceFeedback, false, "None");
}

void UDB_Character::ApplyConstrictionDamage(AController* KillerPlayer, float Damage, float constrictionStatusTimeLeft, float constrictionDamageTimeLeft)
{
	if (!MyCharacter) { return; }

	MyCharacter->SetConstrictionStatus(KillerPlayer, Damage, constrictionStatusTimeLeft, constrictionDamageTimeLeft);
	MyCharacter->MyPlayerController->ClientPlayForceFeedback(DamageForceFeedback, false, "None");
}


void UDB_Character::ApplyHurtShout(float Damage){

	if (!MyCharacter) { return; }

	int32 Random = FMath::RandRange(0, 2);

	if(Damage > 25){
		MyCharacter->PlaySound_Server(MyCharacter->HurtCue, NULL, false, Damage / 50);
	}
	else if(Random == 0){
		MyCharacter->PlaySound_Client(MyCharacter->HurtCue, NULL, false, Damage / 50);
	}
}

void UDB_Character::ZeroGravityStatus(float Time)
{
	MyCharacter->TemporalZeroGravity(Time);
}

bool UDB_Character::IsPawnDied() {
	return MyCharacter->Life <= 0;
}

void UDB_Character::Killed(AConceptGameMode* MyGameMode, AController* KillerPlayer, AController* KilledPlayer, FAbilityData AbilityData){
	MyGameMode->Killed(KillerPlayer, KilledPlayer, AbilityData);
}


void UDB_Character::ApplyRadialImpulse(AController* KillerPlayer, FVector Location, float Radius, float Force, float TimeToStop) {
	AConceptCharacter* EnemyCharacter = Cast<AConceptCharacter>(KillerPlayer->GetPawn());
	EnemyCharacter->MakeRadialForce(Radius, Force, TimeToStop);
}

void UDB_Character::ApplyHitAnim(float Time, float Damage) {
	MyCharacter->OnCharacterHitted(Time);
	MyCharacter->MyPlayerController->ClientPlayForceFeedback(DamageForceFeedback, false, "None");
}
