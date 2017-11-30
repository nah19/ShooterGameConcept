// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "DB_YBlinn.h"


UDB_YBlinn::UDB_YBlinn(){
}

void UDB_YBlinn::Initialize(AActor* ActorToApplybehavior){
    MyMinion = Cast<AYBlinnPawn>(ActorToApplybehavior);
    //if (!MyMinion) {
    //	MyMinion = Cast<ASpeeder>(ActorToApplybehavior);
    //}
}


void UDB_YBlinn::ApplyLifeDamage(float Damage, TSubclassOf<UDamageType> DamageType) {
    
    MyMinion->Health = FMath::Clamp(MyMinion->Health - Damage, 0.0f, MyMinion->MaxHealt);
}

bool UDB_YBlinn::IsPawnDied(){
    return (MyMinion->Health <= 0);
}

void UDB_YBlinn::Killed(AConceptGameMode* MyGameMode, AController* KillerPlayer, AController* KilledPlayer, FAbilityData AbilityData){
    
    MyMinion->Death();
}
