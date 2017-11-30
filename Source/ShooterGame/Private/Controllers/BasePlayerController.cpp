// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "NetGameInstance.h"
#include "UW_HUD.h"
#include "BasePlayerController.h"



ABasePlayerController::ABasePlayerController() {

}

void ABasePlayerController::SetupInputComponent(){
	Super::SetupInputComponent();

	UNetGameInstance* NetGameInstance = Cast<UNetGameInstance>(GetGameInstance());
    if(NetGameInstance){
        bIsUsingGamepad = NetGameInstance->bIsUsingGamepad;
    }

	
	InputComponent->BindAction("AnyGamepad", IE_Pressed, this, &ABasePlayerController::OnGamepadPressed);
	InputComponent->BindAction("AnyKeyboardCustom", IE_Pressed, this, &ABasePlayerController::OnKeyboardPressed);
	InputComponent->BindAxis("Mouse", this, &ABasePlayerController::MouseMovement);
}

bool ABasePlayerController::ShouldCorrectFOVForSplitScreen(){
	int32 Index = 0;
	int32 NumberOfSplitScreens = this->GetSplitscreenPlayerCount();

	if (NumberOfSplitScreens == 0) { return false; }
	if (NumberOfSplitScreens == 2) { return true; }

	for(int32 i = 0; i < NumberOfSplitScreens; i++){
		APlayerState* PlayerByIndex = this->GetSplitscreenPlayerByIndex(i);
		if(PlayerState == PlayerByIndex){
			Index = i;
		}
	}

	if(NumberOfSplitScreens == 3 && Index == 0){
		return true;
	}

	return false;
}

int32 ABasePlayerController::GetSplitScreenIndex() {
	int32 Count = this->GetSplitscreenPlayerCount();
	if (Count == 0) {
		return 0;
	}

	for (int32 i = 0; i < Count; i++) {
		APlayerState* PlayerByIndex = this->GetSplitscreenPlayerByIndex(i);
		
		if (PlayerState == PlayerByIndex) {
			return i;
		}
	}

	return -1;
}


FVector2D ABasePlayerController::GetSplitScreenDivisors() {
	int32 Count = this->GetSplitscreenPlayerCount();
	
	if (Count == 0) {
		return FVector2D(1,1);
	}

	int32 Width = 1920;
	int32 Height = 1080;

	for (int32 i = 0; i < Count; i++) {
		APlayerState* PlayerByIndex = this->GetSplitscreenPlayerByIndex(i);

		if (PlayerState == PlayerByIndex) {
			switch (i)
			{
				case 0: {
					switch (Count)
					{
						case 2: {
							return FVector2D(0, Height/4);
						}
						case 3: {
							return FVector2D(0, Height / 4);
						}
						case 4: {
							return FVector2D(Width / 4, Height / 4);
						}
					}
				}
				case 1: {
					switch (Count)
					{
						case 2: {
							return FVector2D(0, Height / 4);
						}
						case 3: {
							return FVector2D(Width / 4, Height / 4);
						}
						case 4: {
							return FVector2D(Width / 4, Height / 4);
						}
					}
				}
				case 2: {
					switch (Count)
					{
						case 3: {
							return FVector2D(Width / 4, Height / 4);
						}
						case 4: {
							return FVector2D(Width / 4, Height / 4);
						}
					}
				}
				case 3: {
					return FVector2D(Width / 4, Height / 4);
				}
			}
		}
	}

	return FVector2D(0, 0);
}

void ABasePlayerController::OnKeyboardPressed(){
	//TArray<UUserWidget*> Widgets;

	//GetAllWidgetsOfClass(this, Widgets, UUW_HUD::StaticClass(), true);

	//for (int32 i = 0; i < Widgets.Num(); i++) {
	//	UUW_HUD* Widget = Cast<UUW_HUD>(Widgets[i]);
	//	Widget->SetFocus();
	//}
	OnGamepadUseChanged(false);

	
}

void ABasePlayerController::OnGamepadPressed(){
	//TArray<UUserWidget*> Widgets;
	//
	//GetAllWidgetsOfClass(this, Widgets, UUW_HUD::StaticClass(), true);

	//for(int32 i = 0; i < Widgets.Num(); i ++){
	//	UUW_HUD* Widget = Cast<UUW_HUD>(Widgets[i]);
	//	Widget->SetFocus();
	//}

	OnGamepadUseChanged(true);
}

void ABasePlayerController::AddWidget(UUserWidget* Widget){
	EnabledWidgets.Add(Widget);
}

void ABasePlayerController::RemoveWidget(UUserWidget* Widget){
	EnabledWidgets.Remove(Widget);
}

void ABasePlayerController::MouseMovement(float InputDelta) {
	//SpeedOfMouse = FMath::Clamp(FMath::Lerp(SpeedOfMouse, SpeedOfMouse + InputDelta, 0.5f), 0.0f, 10.0f);
	if(InputDelta != 0){
		OnGamepadUseChanged(false);
	}	
}


void ABasePlayerController::OnGamepadUseChanged(bool IsUsingGamepad) {
	if (bIsUsingGamepad != IsUsingGamepad) {
		
		SetOSCursorVisible(!IsUsingGamepad);
		bShowMouseCursor = !IsUsingGamepad;
		bIsUsingGamepad = IsUsingGamepad;

#if PLATFORM_PS4
		bIsUsingGamepad = true;
#endif

		UNetGameInstance* NetGameInstance = Cast<UNetGameInstance>(GetGameInstance());
		NetGameInstance->bIsUsingGamepad = bIsUsingGamepad;
	}
}



void ABasePlayerController::OnLeaveGame_Implementation(bool leaver) {
	UNetGameInstance* NetGameInstance = Cast<UNetGameInstance>(GetGameInstance());
	if (NetGameInstance) {
		
		if (PlayerState) {
			PlayerState->Destroy();
		}
		NetGameInstance->DestroySessionAndLeaveGame();
	}
}

void ABasePlayerController::Destroyed() {
	Super::Destroyed();
	//OnLeaveGame();
}

void ABasePlayerController::OnDestroySession_Implementation() {
	UNetGameInstance* Instance = Cast<UNetGameInstance>(GetGameInstance());
	Instance->DestroySessionAndLeaveGame();
}


void ABasePlayerController::GetAllWidgetsOfClass(UObject* WorldContextObject, TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly)
{
	//Prevent possibility of an ever-growing array if user uses this in a loop
	FoundWidgets.Empty();

	if (!WidgetClass || !WorldContextObject)
	{
		return;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	if (!World)
	{
		return;
	}

	for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		UUserWidget* LiveWidget = *Itr;

		// Skip any widget that's not in the current world context.
		if (LiveWidget->GetWorld() != World)
		{
			continue;
		}

		// Skip any widget that is not a child of the class specified.
		if (!LiveWidget->GetClass()->IsChildOf(WidgetClass))
		{
			continue;
		}

		if (TopLevelOnly)
		{
			if (LiveWidget->IsInViewport())
			{
				FoundWidgets.Add(LiveWidget);
			}
		}
		else
		{
			FoundWidgets.Add(LiveWidget);
		}
	}
}
