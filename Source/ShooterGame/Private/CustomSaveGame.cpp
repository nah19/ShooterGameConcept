// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "CustomSaveGame.h"


UCustomSaveGame::UCustomSaveGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {


}

void UCustomSaveGame::ChangeLocalization(FString target)
{
	FInternationalization::Get().SetCurrentCulture(target);

	//FString MyCommandString = "culture=";

	//FString Final = MyCommandString + target;
	//
	//if (LocalPlayerController) {
	//	LocalPlayerController->ConsoleCommand(*Final, true);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ChangeLocalization %s"), *Final));
	//}
	//else {
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ChangeLocalization FAIL")));
	//}
}

void UCustomSaveGame::LoadLocalization()
{
	FInternationalization::Get().LoadAllCultureData();
	FCultureRef Culture = FInternationalization::Get().GetDefaultCulture();
	FCultureRef Culture2 = FInternationalization::Get().GetCurrentCulture();
	FCultureRef Culture3 = FInternationalization::Get().GetInvariantCulture();
	
	GEngine->AddOnScreenDebugMessage(-1, 105.0f, FColor::Red, FString::Printf(TEXT("GetDefaultCulture %s"), *Culture->GetTwoLetterISOLanguageName()));
	GEngine->AddOnScreenDebugMessage(-1, 105.0f, FColor::Red, FString::Printf(TEXT("GetCurrentCulture %s"), *Culture2->GetTwoLetterISOLanguageName()));
	GEngine->AddOnScreenDebugMessage(-1, 105.0f, FColor::Red, FString::Printf(TEXT("GetInvariantCulture %s"), *Culture3->GetTwoLetterISOLanguageName()));
	GEngine->AddOnScreenDebugMessage(-1, 105.0f, FColor::Red, FString::Printf(TEXT("GetNativeLanguage %s"), *Culture->GetNativeName()));

	FInternationalization::Get().SetCurrentCulture(*Culture->GetTwoLetterISOLanguageName());
}


void UCustomSaveGame::ApplyControlSettings(APlayerController* PlayerController, float MouseSensitivity, float XSensitivity, float YSensitivity, float XSensitivityGamepad, float YSensitivityGamepad, bool XInvert, bool YInvert) {
	PlayerController->PlayerInput->SetMouseSensitivity(MouseSensitivity);



		TArray<struct FInputAxisConfigEntry> AxisConfigArray = PlayerController->PlayerInput->AxisConfig;

		for (int32 i = 0; i < PlayerController->PlayerInput->AxisConfig.Num(); i++) {
			if (PlayerController->PlayerInput->AxisConfig[i].AxisKeyName == "MouseX") {
				PlayerController->PlayerInput->AxisConfig[i].AxisProperties.Exponent = XSensitivity;
				PlayerController->PlayerInput->AxisConfig[i].AxisProperties.bInvert = XInvert;
			}
		}

	//for (int32 i = 0; i < AxisConfigArray.Num(); i++) {
	//	if (AxisConfigArray[i].AxisKeyName == "MouseX") {
	//		AxisConfigArray[i].AxisProperties.Sensitivity = XSensitivity;
	//		AxisConfigArray[i].AxisProperties.bInvert = XInvert;
	//	}
	//}

	//for (int32 i = 0; i < AxisConfigArray.Num(); i++) {
	//	if (AxisConfigArray[i].AxisKeyName == "MouseY") {
	//		AxisConfigArray[i].AxisProperties.Sensitivity = XSensitivity;
	//		AxisConfigArray[i].AxisProperties.bInvert = XInvert;
	//	}
	//}

	//PlayerController->PlayerInput->AxisConfig = AxisConfigArray;
	//TArray<FInputAxisKeyMapping> KeyMappingX = PlayerController->PlayerInput->AxisMappings;
	//for (int32 i = 0; i < KeyMappingX.Num(); i++) {

	//	if (KeyMappingX[i].AxisName == "TurnX") {
	//		FInputAxisProperties PropertiesX;
	//		PropertiesX.bInvert = XInvert;
	//		PropertiesX.Sensitivity = XSensitivity;
	//		PlayerController->PlayerInput->SetAxisProperties(KeyMappingX[i].Key, PropertiesX);
	//	}


	//	if (KeyMappingX[i].AxisName == "TurnY") {
	//		FInputAxisProperties PropertiesX;
	//		PropertiesX.bInvert = XInvert;
	//		PropertiesX.Sensitivity = XSensitivity;
	//		PlayerController->PlayerInput->SetAxisProperties(KeyMappingX[i].Key, PropertiesX);
	//	}



	//}

	//TArray<FInputAxisKeyMapping> KeyMappingY = PlayerController->PlayerInput->AxisMappings;
	//for (int32 i = 0; i < KeyMappingY.Num(); i++) {
	//	if (KeyMappingY[i].AxisName == "TurnY") {
	//		FInputAxisProperties PropertiesY;
	//		PropertiesY.bInvert = YInvert;
	//		PropertiesY.Sensitivity = YSensitivity;
	//		PlayerController->PlayerInput->SetAxisProperties(KeyMappingY[i].Key, PropertiesY);
	//	}
	//}
}

FString UCustomSaveGame::GetResolutionCommand(int32 XResolution, int32 YResolution, FString Mode){
    
    FString MyCommandString = "r.SetRes ";
    FString ResolutionX = FString::FromInt(XResolution);
    FString ResolutionY = FString::FromInt(YResolution);
 
#if PLATFORM_MAC
    if(Mode.Equals("f")){
        Mode = "wf";
    }
#endif
    
    FString Final = MyCommandString + ResolutionX + "x" +  ResolutionY + Mode;
    
    return Final;
}
