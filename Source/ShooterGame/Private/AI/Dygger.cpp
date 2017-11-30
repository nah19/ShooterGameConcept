// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "Dygger.h"
#include "DB_Minion.h"
#include "MinionPathFollowingComponent.h"
#include "HiveGravityZone.h"
#include "CustomCharacterMovementComponent.h"


ADygger::ADygger(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
	PrimaryActorTick.bCanEverTick = true;
	DifficultPoints = 6;
	MaxHealt = 100;
	Health = MaxHealt;
	Speed = 30;
	MapPawnOrientations.SetNumUninitialized(2);
}

void ADygger::NewGeneration() {
	generation += 1;
	Health = MaxHealt;
	UE_LOG(LogTemp, Log, TEXT("Generation: %d"), generation);

}

void ADygger::OnLand(){
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("updatinggg!!!!!!"));

    AMinionController* MyController = Cast<AMinionController>(GetController());
    if(MyController){
        UMinionPathFollowingComponent* PathFollowing = Cast<UMinionPathFollowingComponent>(MyController->GetPathFollowingComponent());
        if(!PathFollowing) return;
        PathFollowing->UpdatePathSegment();
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("updatingpathsegment!!!!!!"));
    }
}
