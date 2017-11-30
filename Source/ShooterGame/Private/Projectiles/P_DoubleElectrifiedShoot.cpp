// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "P_DoubleElectrifiedShoot.h"


AP_DoubleElectrifiedShoot::AP_DoubleElectrifiedShoot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
    PrimaryActorTick.bCanEverTick = true;
    
}

void AP_DoubleElectrifiedShoot::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
    APawn* Character = Cast<APawn>(GetOwner());
    APawn* CharacterToAffect = Cast<APawn>(ActorToAffectParam);
    
    if (Character) {
        UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
        
    }
    
    Destroy();
    
}

