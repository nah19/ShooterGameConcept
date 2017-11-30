// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ConceptCharacter.h"
#include "UW_HUD.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UUW_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
		bool IsPS4();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void InitParentWidged(TSubclassOf<UUserWidget> ParentWidgetClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void InitParentActorWidged(UUserWidget* ParentWidgetClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnGamepadUseChanged(bool IsUsingGamepad);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void InitDamageIndicator(APawn* CharacterDamager);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void KillEvent(const FString& KillerGameTag, const FString& VictimGameTag, const FString& KillerNameAbility, int32 KillerTeam, int32 VictimTeam);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnCrossPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnEnterPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnCrossReleased();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnCirclePressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnBackSpacePressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnSquarePressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnTrianglePressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnUpArrowPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnUpPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnDownArrowPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnDownPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnLeftArrowPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnLeftPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnRightArrowPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnRightPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnR1Pressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnL1Pressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnR2Pressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnL2Pressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void OnStartPressed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void SetFocus();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "HUD")
		int32 GamepadIndex = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "HUD")
		bool bIsUsingGamepad = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int32 ClampMax = 10;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent & InMouseEvent) override;

	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

};


