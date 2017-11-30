// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Medal.h"



UMedal::UMedal()
{
	MedalName = NSLOCTEXT("Medals", "UMedal", "NombrePorDefecto");
	Description = NSLOCTEXT("Medals", "UMedal_Des", "Descripcion por defecto");

	Multiplier = 1.5f;
	TypeIndex = 0;
}



UMedal* UMedal::GetAchievedMedal(AConceptStats* Stats){
	return NULL;
}