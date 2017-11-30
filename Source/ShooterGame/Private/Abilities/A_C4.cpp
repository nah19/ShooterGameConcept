// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "A_C4.h"


AA_C4::AA_C4()
	:Super()
{

}

void AA_C4::PerformSpecialBehavour() {
	for (TActorIterator<AP_C4> Explosives(GetWorld()); Explosives; ++Explosives)
	{
		AP_C4 *Explosive = *Explosives;
		if (Explosive) {
			if (GetOwner() == Explosive->GetOwner()) {
				Explosive->Activate();
			}
			//AConceptPlayerController* Controller = Cast<AConceptPlayerController>(Explosive->GetOwner()->GetInstigatorController());
			//if (Controller && Controller->IsLocalPlayerController()) {
			//	Explosive->Activate();
			//}
		}
	}
}

void AA_C4::UseAbility()
{
	Super::UseAbility();
}

