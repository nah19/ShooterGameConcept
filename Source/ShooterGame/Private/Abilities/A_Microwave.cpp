// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_AssaultWeapon.h"
#include "D_Microwave.h"
#include "A_Microwave.h"

AA_Microwave::AA_Microwave()
	:Super()
{
	
}


//void AA_Microwave::DamageObjectsHitted_Implementation(bool IsSomethingHitted, const TArray<FHitResult>& OutHits) {
//	if (IsSomethingHitted) {
//		for (int32 i = 0; i < OutHits.Num(); i++) {
//			FHitResult const& Hit = OutHits[i];
//
//			AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
//			AConceptPlayerController* MyPlayerController = Cast<AConceptPlayerController>(MyCharacter->GetController());
//			if (Hit.GetActor() && Hit.GetActor()->bCanBeDamaged)
//			{
//
//				UGameplayStatics::ApplyDamage(Hit.GetActor(), ContinuousDamage, (AController*)MyPlayerController, this, UD_AssaultWeapon::StaticClass());
//
//
//			}
//		}
//	}
//}
//
//
//// --------------------DEPRECATED FUNCTIONS--------------------
//// ------------------------------------------------------------
//// Old functions for the microwave secondary ability. New one 
//// uses the damage UObjects.
//// ------------------------------------------------------------
//
//void AA_Microwave::StopAbility()
//{
//	Super::StopAbility();
//}
//
//bool AA_Microwave::DamageObjectsHitted_Validate(bool IsSomethingHitted, const TArray<FHitResult>& OutHits) { return true; }
//
//void AA_Microwave::ClearPlayers()
//{
//	if (CharactersHit.Num() <= 0) 
//	{
//		return;
//	}
//
//	for (int32 i = 0; i < CharactersHit.Num(); i++)
//	{
//		CharactersHit[i]->CanShoot(true);
//	}
//	CharactersHit.Empty();
//}
//
//void AA_Microwave::PlayerCanShoot(AConceptCharacter* Player, bool bCanShoot)
//{
//	Player->CanShoot(bCanShoot);
//}
//
//void AA_Microwave::SendClear_Implementation()
//{
//	ClearPlayers();
//	SendClear_Clients();
//}
//
//bool AA_Microwave::SendClear_Validate()
//{
//	return true;
//}
//
//void AA_Microwave::SendClear_Clients_Implementation()
//{
//	ClearPlayers();
//}