// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "A_HealinLink.h"


AA_HealinLink::AA_HealinLink() {

}

void AA_HealinLink::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	AConceptCharacter* OwnerCharacter = Cast<AConceptCharacter>(GetOwner());
	if (OwnerCharacter) {
		if(Cast<AConceptCharacter>(OwnerCharacter->SelectedTarget) != NULL){
			HealTarget = Cast<AConceptCharacter>(OwnerCharacter->SelectedTarget);
		}
		
		ParticleComponent->SetActorParameter(FName("self"), this);
		ParticleComponent->SetActorParameter(FName("Partner"), HealTarget);
		if (Cast<AConceptCharacter>(OwnerCharacter->SelectedTarget) && OwnerCharacter->bWantsFire) {
			if (!ParticleComponent->IsActive()) {UseAbility();}
		}
		else if(ParticleComponent->IsActive() && OwnerCharacter->IsLocallyControlled()){
			SetActivatedParticle(false);
		}
	}
}

void AA_HealinLink::ApplyDamage_Implementation(AConceptCharacter* OwnerCharacter)
{
	UGameplayStatics::ApplyDamage(HealTarget, DamageBase, OwnerCharacter->GetController(), this, DamageTypeClass);
}

bool AA_HealinLink::ApplyDamage_Validate(AConceptCharacter* OwnerCharacter)
{
	return true;
}

void AA_HealinLink::UseAbility() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UseAbility"));
	AConceptCharacter* OwnerCharacter = Cast<AConceptCharacter>(GetOwner());
	if (OwnerCharacter) {
		HealTarget = Cast<AConceptCharacter>(OwnerCharacter->SelectedTarget);
		if (HealTarget) {
			ApplyDamage(OwnerCharacter);

			//SetActivatedParticle(true);
		}
		else{
			StateOfAbility = EStateOfAbility::RE_READY;
			return;
		}
	}
	Super::UseAbility();
}

void AA_HealinLink::StopAbility() {
	Super::StopAbility();
}