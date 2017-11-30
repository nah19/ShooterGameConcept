
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BasePlayerController.h"
#include "PC_MainMenu.generated.h"

/**
*
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate);

UCLASS( )
class SHOOTERGAME_API APC_MainMenu : public ABasePlayerController {
	GENERATED_BODY( )

	APC_MainMenu();





private:
	virtual void SetupInputComponent() override;
};