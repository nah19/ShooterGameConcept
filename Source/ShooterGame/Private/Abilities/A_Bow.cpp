// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "A_Bow.h"
#include "P_Arrow.h"


AA_Bow::AA_Bow()
	:Super()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AA_Bow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// Charge the Arrow
	if (bChargeArrow)
	{
		ChargeArrow(DeltaSeconds);
	}

	if (!bSetSpeed)
		return;
	
	for (TActorIterator<AP_Arrow> Arrows(GetWorld()); Arrows; ++Arrows)
	{
		AP_Arrow *Arrow = *Arrows;
		if (Arrow) {
			if (GetOwner() == Arrow->GetOwner() && !Arrow->bSpeedSetted) {
				Arrow->SetInitialSpeed(FinalForce);
			}
		}
	}
	//for (int32 i = 0; i < PrimaryAbilityProjectiles.Num(); i++)
	//{
	//	AP_Arrow* Arrow = Cast<AP_Arrow>(PrimaryAbilityProjectiles[i]);
	//	if (Arrow)
	//	{
	//		AConceptPlayerController* Controller = Cast<AConceptPlayerController>(Arrow->GetOwner()->GetInstigatorController());
	//		if (Controller && Controller->IsLocalPlayerController() && !Arrow->bSpeedSetted)
	//		{

	//			Arrow->SetInitialSpeed(FinalForce);

	//			//bSetSpeed = false;
	//		}
	//	}
	//}
}

void AA_Bow::BeginPlay()
{
	Super::BeginPlay();
	CurrentForce = MinThrowForce;
	bChargeArrow = false;
	bSetSpeed = false;
}


void AA_Bow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AA_Bow::StartAbility()
{
}

void AA_Bow::StopAbility()
{
	/*if (Type != 0)
		return;*/

	if (bChargeArrow)
	{
		ThrowArrow();
	}

	bChargeArrow = false;

	Super::StopAbility();

	FinalForce = CurrentForce;
	CurrentForce = 0.0f;
}

void AA_Bow::ChargeArrow(float DeltaSeconds)
{
	CurrentForce += ForceChargeSpeed * DeltaSeconds;

	if (CurrentForce > MaxThrowForce)
	{
		CurrentForce = MaxThrowForce;
	}

}

void AA_Bow::ThrowArrow()
{
	// Prepare the arrow
	bShootArrow = true;
	bChargeArrow = false;
	bSetSpeed = true;

	// Throw it
	FireAbility();
}
