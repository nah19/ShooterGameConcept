// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GameFramework/PlayerController.h"
#include "BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "StaticFunctionLibrary.h"
#include "ConceptPlayerController.generated.h"

/**
 *
 */

class UUW_HUD;

UENUM(BlueprintType)
enum class EControllerInput : uint8 {
	RE_NO_INPUT 	UMETA(DisplayName = "RE_NO_INPUT"),
	RE_NO_INITIALIZED 	UMETA(DisplayName = "RE_NO_INITIALIZED"),
	RE_STANDARD_INPUT 	UMETA(DisplayName = "RE_STANDARD_INPUT"),
	RE_SPECTATOR 	UMETA(DisplayName = "RE_SPECTATOR"),
	RE_MENU 	UMETA(DisplayName = "RE_MENU"),
};

UENUM(BlueprintType)
enum class EHUDStateEnum : uint8 {
	RE_PLAYER_LOADER 	UMETA(DisplayName = "RE_PLAYER_LOADER"),
	RE_STATISTICS 	UMETA(DisplayName = "RE_STATISTICS"),
	RE_DEATHSTATISTICS 	UMETA(DisplayName = "RE_DEATHSTATISTICS"),
	RE_INGAME_HUD 	UMETA(DisplayName = "RE_INGAME_HUD"),
	RE_NO_HUD 	UMETA(DisplayName = "RE_NO_HUD"),
	RE_RESULTS 	UMETA(DisplayName = "RE_RESULTS"),
	RE_UPGRADES UMETA(DisplayName = "RE_UPGRADES"),
	RE_DAMAGE UMETA(DisplayName = "RE_DAMAGE"),
	RE_STEALTH UMETA(DisplayName = "RE_STEALTH"),
	RE_FINALSTATISTICS UMETA(DisplayName = "RE_FINALSTATISTICS"),

	
};

UCLASS()
class SHOOTERGAME_API AConceptPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

public:

	AConceptPlayerController();

	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "Gamepad")
		bool bUseGamepad;
	UPROPERTY()
		FVector2D VirtualMousePosition;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
	class AConceptCharacter *MyCharacter;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
	class AConceptGameMode *MyGameMode;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
	class AConceptGameState *MyGameState;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
	class AConceptPlayerState *MyPlayerState;
	UPROPERTY(Replicated, visibleinstanceonly, blueprintreadwrite, Category = "Gamemode")
		UUserWidget* DamagedHUD;

    //Replay fiunctions
    UFUNCTION(BlueprintCallable, Category="DemoRec")
    void DemoRec();
    UFUNCTION(BlueprintCallable, Category="DemoRec")
    void DemoStop();
    UFUNCTION(BlueprintCallable, Category="DemoRec")
    void DemoPlay();
	
	//UFUNCTION(reliable, Client, BlueprintCallable, Category = "Ability")
	void OnLeaveGame_Implementation(bool leaver = true) override;

	UFUNCTION()
		void PlayForceFeedback(float Damage);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		UUserWidget* InGameHUD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		UUserWidget* InGameStatistics;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		UUserWidget* DeathStatistics;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		UUserWidget* PauseMenuHUD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		UUserWidget* FinalStatistics;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
		UUserWidget* PlayerLoader;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
		UUserWidget* HealthHUD;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
		UUserWidget* InvisibleHUD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> InGameHUDClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> PauseMenuClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUW_HUD> DamageIndicatorClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> DeathStatisticsClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> InGameStatisticsClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> FinalStatisticsClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> PlayerLoaderClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> DamageHUDClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> HealthHUDClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
		TSubclassOf<class UUserWidget> StealthHUDClass;
	UPROPERTY(Replicated, visibleanywhere, blueprintreadonly, Category = "Gamemode")
		EHUDStateEnum HUDState;
	UPROPERTY()
		FVector DirectionMouse;
	UPROPERTY(visibleanywhere, blueprintreadonly, Category = "Gamemode")
		EControllerInput ControllerInput;

	UFUNCTION()
		void BindKeysToChangeAbility();
	UFUNCTION()
		void BindGeneralKeys();
	UFUNCTION()
		void BindKeysToAttack();
	UFUNCTION()
		void BindKeysPauseMenu();
	UFUNCTION()
		void ChangeResolutionTo(const FString &ScreenResolutionCommand);
	UFUNCTION()
		void OnTABPressed();
	UFUNCTION()
		void OnTABReleased();
	UFUNCTION()
		void RemoveAllHudElements();
	UFUNCTION(Client, Reliable)
		void CalculateViewportSize();
	UFUNCTION()
		void BindKeysToShowStatistics();
	UFUNCTION()
		void InitWidget(TSubclassOf<class UUserWidget> WidgetClass, UUserWidget* &Widget, float ZOrder = -100, bool bShouldAddToScreen = true);
	UFUNCTION()
		void RemoveWidget(UUserWidget* &Widget);
	UFUNCTION(Client, Reliable)
		void InitInGameHUD();
	UFUNCTION(Client, Reliable)
		void InitPauseMenu();
	UFUNCTION(Client, Reliable)
		void InitFinalStatisticsHUD(int32 Winner);
	UFUNCTION(Client, Reliable)
		void InitDeathStatisticsHUD();
	UFUNCTION(Client, Reliable)
		void RemoveDeathStatisticsHUD();
	UFUNCTION(Client, Reliable)
		void RemovePauseMenu();
	UFUNCTION(Client, Reliable)
		void RemoveFinalStatisticsHUD();
	UFUNCTION(Client, Reliable)
		void RemoveInGameHUD();
	UFUNCTION(Client, Reliable)
		void InitInGameStatisticsHUD();
	UFUNCTION(Client, Reliable)
		void RemoveInGameStatisticsHUD();
	UFUNCTION(Client, Reliable)
		void RemoveInGameStealthHUD();
	UFUNCTION(Client, Reliable)
		void InitPlayerLoaderHUD();
	UFUNCTION(Client, Reliable)
		void RemovePlayerLoaderHUD();
	UFUNCTION()
		void BindKeysToMoveCameraFree();
	UFUNCTION(Client, Reliable)
		void RemoveDamageHUD();
	UFUNCTION(Client, Reliable)
		void RemoveHealthHUD();
	UFUNCTION()
		void SetMousePositionX(float LocationX);
	UFUNCTION()
		void SetMousePositionY(float LocationY);
	UFUNCTION(BlueprintCallable, Category = "HUD")
		void PauseMenuManager();
	UFUNCTION()
		void OnMapLogin();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void AddWidgetToViewport(UUserWidget* Widget, float ZOrder, bool bShouldAddToScreen);


	virtual void PostInitializeComponents() override;
	virtual void OnRep_Pawn() override;
	virtual void Tick(float DeltaSeconds) override;

	bool doOnce=false;

public:

	UPROPERTY()
		FVector2D Mouse2DLocation;
	UPROPERTY()
		FVector Mouse3DLocation;
	UPROPERTY()
		FVector2D CenterOfViewport;
	UPROPERTY()
		FVector2D ViewportSize;

	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyCharacter(AConceptCharacter* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyPlayerState(AConceptPlayerState* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyGameMode(AConceptGameMode* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyGameState(AConceptGameState* Char);
	UFUNCTION(BlueprintCallable, Category = "Ability")
		void BindKeysToMoveCharacter();
	UFUNCTION(Client, Reliable)
		void InitInGameDamageHUD();
	UFUNCTION(Client, Reliable)
		void InitInGameStealthHUD();
	UFUNCTION(Client, Reliable)
		void InitInGameHealthHUD();
	UFUNCTION(BlueprintCallable, Category = "Ability")
		FRotator CalculateRotToCursorPosition(FVector StartLocationParam, float UpperAngleLimit, float LowerAngleLimit);
	UFUNCTION(BlueprintCallable, Category = "Ability")
		FVector CalculateCursorPosition();
	UFUNCTION()
		void CalculateMouse2DLocation();
	UFUNCTION()
		void InitState();
	UFUNCTION()
		void InitOwnState();
	UFUNCTION()
		void InitStrangerState();
	UFUNCTION()
		void InitAttributtes();
	UFUNCTION()
		void InitInputs();
	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Input")
		void InputManager(EControllerInput MyControllerInput);
	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Ability")
		void HUDManager(EHUDStateEnum NextHUDState);
	UFUNCTION(reliable, server, WithValidation)
		void ClientSetSpectatorCamera(FVector CameraLocation, FRotator CameraRotation);
	UFUNCTION(reliable, NetMulticast)
		void AdjustClientSetSpectatorCamera(FVector CameraLocation, FRotator CameraRotation);
	UFUNCTION()
		void CalculateWorldPositionOfMouse();
	UFUNCTION(Client, Reliable, WithValidation)
		void OnDamageReceived(APawn* CharacterDamager);
	UFUNCTION()
		void KillEventToHUD(FString KillerGameTag, FString VictimGameTag, FString KillerNameAbility, int32 KillerTeam, int32 VictimTeam);
	UFUNCTION(BlueprintCallable, Category = "Character")
		FVector2D CalculateAnimationAimPoint(FVector StartLocationParam, FRotator FloorRotation);
	UFUNCTION()
		AConceptCharacter* GetConceptCharacter();
	UFUNCTION()
		AConceptPlayerState* GetPlayerState();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
		void ShowInGameHUD();
	//UFUNCTION(BlueprintCallable, Category = "Character")
	//	FVector2D GetNativeResolution();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void MakeTeleport();
		
private:

	UFUNCTION(Server, Reliable, WithValidation)
		void ShowResults();

	virtual void SetupInputComponent() override;


};
