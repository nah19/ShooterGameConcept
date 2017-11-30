// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "A_TurretShoot.h"
#include "ConceptPlayerController.h"
#include "A_Turrets.h"

AA_Turrets::AA_Turrets() {

}

void AA_Turrets::UseAbility(){
	Super::UseAbility();

	PlaySound();
/*
	if (Type == 0) {

		int32 lookAt = 0;
		AConceptCharacter* MyChar = Cast<AConceptCharacter>(GetOwner());
		if (MyChar){
			float x = 0, y = 0;
			MyChar->MyPlayerController->GetMousePosition(x, y);
			x = x / GEngine->GameViewport->Viewport->GetSizeXY().X;
			OrientationY = x > 0.5 ? 90 : -90;
		}
		AA_TurretShoot* SpawnedTurret = GetWorld()->SpawnActor<AA_TurretShoot>(Turret, GetOwner()->GetActorLocation() + FVector(0, 0, 100), FRotator(180, OrientationY, 0));
		SpawnedTurret->SetActorScale3D(FVector(2,2,2));
		SpawnedTurret->TeamNumber = 0;
		SpawnedTurrets.Add(SpawnedTurret);
		return;

	}*/ //if (Type == 1){

	//	for (auto Itr(SpawnedTurrets.CreateIterator()); Itr; Itr++)
	//	{
	//		(*Itr)->Explode();
	//	}
	//	SpawnedTurrets.Empty();
	//}
}