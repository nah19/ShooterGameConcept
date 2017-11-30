// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameFramework/PlayerController.h"

#include "BasePlayerController.generated.h"

class UUW_HUD;

UCLASS()
class SHOOTERGAME_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ABasePlayerController();

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "HUD")
		bool bIsUsingGamepad;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "HUD")
		float SpeedOfMouse = 0.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "HUD")
		TArray<UUserWidget*> EnabledWidgets;

	UFUNCTION(reliable, Client, BlueprintCallable, Category = "Ability")
		virtual void OnLeaveGame(bool leaver = true);

	UFUNCTION(Client, Reliable)
		void OnDestroySession();

	UFUNCTION(BlueprintCallable,  Category = "HUD")
		void AddWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable,  Category = "HUD")
		void RemoveWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "SteamPlayerState")
		bool ShouldCorrectFOVForSplitScreen();

	UFUNCTION(BlueprintCallable, Category = "SteamPlayerState")
		int32 GetSplitScreenIndex();

	UFUNCTION(BlueprintCallable, Category = "SteamPlayerState")
		FVector2D GetSplitScreenDivisors();

	static FORCEINLINE void SetOSCursorVisible(bool MakeVisible)
	{
		FSlateApplication::Get().GetPlatformApplication()->Cursor->Show(MakeVisible);
	}

	virtual void Destroyed() override;

	void OnKeyboardPressed();

	void OnGamepadPressed();

	void OnGamepadUseChanged(bool IsUsingGamepad);

	void MouseMovement(float InputDelta);

	virtual void SetupInputComponent() override;

	void GetAllWidgetsOfClass(UObject* WorldContextObject, TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly);
};
