// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Steam/SteamStat.h"

USteamStat::USteamStat()
{
	StatName = "Hello";
	StatValue = 0;
}

void USteamStat::InitStat(FString Name)
{
	StatName = Name;
	StatValue = 0.0f;
	//AddToRoot();
}

FString USteamStat::GetStatInfo()
{
	FString String = StatName + ": ";
	String.AppendInt(StatValue);
	return String;
}

