// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "PC_MainMenu.h"
#include "UW_HUD.h"

bool UUW_HUD::IsPS4() {
#if PLATFORM_PS4
	return true;
#endif
	return false;
}

void UUW_HUD::InitDamageIndicator_Implementation(APawn* CharacterDamager) {}

void UUW_HUD::InitParentWidged_Implementation(TSubclassOf<UUserWidget> ParentWidgetClass){}

void UUW_HUD::InitParentActorWidged_Implementation(UUserWidget* ParentWidgetClass) {}

void UUW_HUD::KillEvent_Implementation(const FString& KillerGameTag, const FString& VictimGameTag, const FString& KillerNameAbility, int32 KillerTeam, int32 VictimTeam) {}

void UUW_HUD::OnGamepadUseChanged_Implementation(bool IsUsingGamepad) {
	ABasePlayerController* MyController = Cast<ABasePlayerController>(GetOwningPlayer());

	
	if (MyController->bIsUsingGamepad != IsUsingGamepad) {
		MyController->SetOSCursorVisible(!IsUsingGamepad);
		MyController->bShowMouseCursor = !IsUsingGamepad;
		MyController->bIsUsingGamepad = IsUsingGamepad;
	}
}

FReply UUW_HUD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent){
	//Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	//ABasePlayerController* MyController = Cast<ABasePlayerController>(GetOwningPlayer());
	//OnGamepadUseChanged(InKeyEvent.GetKey().IsGamepadKey());

	//if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom) { OnCrossPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Left) { OnSquarePressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Right) { OnCirclePressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Top) { OnTrianglePressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Down) { OnDownArrowPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Left) { OnLeftArrowPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Right) { OnRightArrowPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_DPad_Up) { OnUpArrowPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_RightShoulder) { OnR1Pressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_LeftShoulder) { OnL1Pressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Gamepad_Special_Right) { OnStartPressed(); }


	//if (InKeyEvent.GetKey() == EKeys::Enter) { OnEnterPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::BackSpace) { OnBackSpacePressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Down) { OnDownPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Left) { OnLeftPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Right) { OnRightPressed(); }
	//if (InKeyEvent.GetKey() == EKeys::Up) { OnUpPressed(); }

	return FReply::Handled();
}

FReply UUW_HUD::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	//Super::NativeOnKeyUp(InGeometry, InKeyEvent);

	//if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom) { OnCrossReleased(); }

	return FReply::Handled();
}

FReply UUW_HUD::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent & InMouseEvent) {
	//Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	//if(InMouseEvent.GetCursorDelta().X != 0 || InMouseEvent.GetCursorDelta().Y != 0){ OnGamepadUseChanged(false);}

	return FReply::Handled();
}


void UUW_HUD::OnCrossPressed_Implementation(){}

void UUW_HUD::OnEnterPressed_Implementation() {}

void UUW_HUD::OnCirclePressed_Implementation(){}

void UUW_HUD::OnBackSpacePressed_Implementation() {}

void UUW_HUD::OnSquarePressed_Implementation(){}

void UUW_HUD::OnTrianglePressed_Implementation(){}

void UUW_HUD::OnUpArrowPressed_Implementation() {}

void UUW_HUD::OnUpPressed_Implementation() {}

void UUW_HUD::OnDownArrowPressed_Implementation() {}

void UUW_HUD::OnDownPressed_Implementation() {}

void UUW_HUD::OnLeftArrowPressed_Implementation() {}

void UUW_HUD::OnLeftPressed_Implementation() {}

void UUW_HUD::OnRightArrowPressed_Implementation() {}

void UUW_HUD::OnRightPressed_Implementation() {}

void UUW_HUD::OnR1Pressed_Implementation(){}

void UUW_HUD::OnL1Pressed_Implementation() {}

void UUW_HUD::OnR2Pressed_Implementation() {}

void UUW_HUD::OnL2Pressed_Implementation() {}

void UUW_HUD::OnStartPressed_Implementation() {}

void UUW_HUD::OnCrossReleased_Implementation() {}



void UUW_HUD::SetFocus_Implementation() { 
	
	//SetKeyboardFocus();
}