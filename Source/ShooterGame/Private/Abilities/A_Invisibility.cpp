// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_Invisibility.h"
#include "A_Invisibility.h"


AA_Invisibility::AA_Invisibility() {
	//TODO: cadencia y municion
}   

void AA_Invisibility::UseAbility(){
	Super::UseAbility();

	//UGameplayStatics::PlaySoundAtLocation(this, SoundOnFire, GetActorLocation(), GetActorRotation());
	SwitchVisibility(0);
}


void AA_Invisibility::SwitchVisibility_Implementation(int32 Visible){
	AConceptCharacter* InvisibleCharacter = Cast<AConceptCharacter>(GetOwner());
	if (InvisibleCharacter) {
		//if (Type == 1) {
			//UGameplayStatics::ApplyRadialDamage(GetWorld(), 1, GetActorLocation(), 50.f, UD_Invisibility::StaticClass(), TArray<AActor*>(), this, InvisibleCharacter->Controller, true, ECC_Visibility);
			UGameplayStatics::ApplyDamage(InvisibleCharacter, 1, InvisibleCharacter->Controller, this, DamageTypeClass);
		//} else {
		//	UGameplayStatics::ApplyDamage(InvisibleCharacter, 1, InvisibleCharacter->Controller, this, UD_Invisibility::StaticClass());

		//}
	}
}

bool AA_Invisibility::SwitchVisibility_Validate(int32 Visible){
	return true;
}