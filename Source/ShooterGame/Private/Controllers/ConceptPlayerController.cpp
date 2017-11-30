// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptCharacter.h"
#include "ConceptSpectatorPawn.h"
#include "ConceptGameMode.h"
#include "WidgetBlueprintLibrary.h"
#include "ConceptPlayerController.h"
#include "UW_HUD.h"
#include <functional>

#define DRAW -1
#define VICTORY 0
#define DEFEAT 1

AConceptPlayerController::AConceptPlayerController() {



	ControllerInput = EControllerInput::RE_NO_INPUT;

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	HUDState = EHUDStateEnum::RE_NO_HUD;
	ViewportSize = FVector2D(720, 480);

	DirectionMouse = FVector::ZeroVector;
	Mouse3DLocation = FVector::ZeroVector;

	NetUpdateFrequency = 20;
}

void AConceptPlayerController::OnRep_Pawn() {
	Super::OnRep_Pawn();
	InitState();
}

void AConceptPlayerController::PostInitializeComponents() {
	Super::PostInitializeComponents();

	InitState();
	InitAttributtes();

}

void AConceptPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InitState();
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(this->Player);
	if (LocalPlayer) {
		FViewport* v = LocalPlayer->ViewportClient->Viewport;
		FIntPoint LocalMousePosition;
		v->GetMousePos(LocalMousePosition);
		VirtualMousePosition = (FVector2D)LocalMousePosition;
	}
	InitInputs();
}

void AConceptPlayerController::OnMapLogin() {
	InitWidget(PlayerLoaderClass, PlayerLoader, 100.0f, false);
	MyPlayerState->FillVars();
	InitState();
	InitAttributtes();
}

void AConceptPlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (IsLocalController() && !doOnce && MyPlayerState) {
		InitWidget(PlayerLoaderClass, PlayerLoader, 100.0f, false);
		MyPlayerState->FillVars();
		doOnce = true;
	}
}

void AConceptPlayerController::InitAttributtes() {
	if (MyGameMode) {
		MyGameMode->CustomPostLogin(this);
	}
	PlayerCameraManager->bUseClientSideCameraUpdates = false;
}

void AConceptPlayerController::InitState() {
	InitOwnState();
	InitStrangerState();
}

void AConceptPlayerController::InitOwnState() {
	if (!MyCharacter) { MyCharacter = Cast<AConceptCharacter>(GetCharacter()); }
	if (!MyPlayerState) { MyPlayerState = Cast<AConceptPlayerState>(PlayerState); }
	if (!MyGameMode) { MyGameMode = Cast<AConceptGameMode>(GetWorld()->GetAuthGameMode()); }
	if (!MyGameState) { MyGameState = Cast<AConceptGameState>(GetWorld()->GetGameState()); }
}

void AConceptPlayerController::InitStrangerState() {
	if (MyCharacter) { MyCharacter->SetMyPlayerController(this); }
	if (MyPlayerState) { MyPlayerState->SetMyPlayerController(this); }
}

void AConceptPlayerController::InitInputs() {
	if (InputComponent == NULL){ return; }
	if (ControllerInput == EControllerInput::RE_NO_INPUT){
		InputManager(EControllerInput::RE_SPECTATOR);
	}
}


void AConceptPlayerController::SetMyCharacter_Implementation(AConceptCharacter* Char){MyCharacter = Char;}
void AConceptPlayerController::SetMyPlayerState_Implementation(AConceptPlayerState* PState){MyPlayerState = PState;}
void AConceptPlayerController::SetMyGameMode_Implementation(AConceptGameMode* Game){MyGameMode = Game;}
void AConceptPlayerController::SetMyGameState_Implementation(AConceptGameState* State){MyGameState = State;}
bool AConceptPlayerController::SetMyCharacter_Validate(AConceptCharacter* Char) {return true;}
bool AConceptPlayerController::SetMyPlayerState_Validate(AConceptPlayerState* Char) {return true;}
bool AConceptPlayerController::SetMyGameMode_Validate(AConceptGameMode* Char) {return true;}
bool AConceptPlayerController::SetMyGameState_Validate(AConceptGameState* Char) {return true;}

void AConceptPlayerController::OnLeaveGame_Implementation(bool leaver) {
	if (leaver && MyPlayerState) {
		MyPlayerState->SetPlayerState(EStateEnum::RE_LEAVER);
	}
	UNetGameInstance* NetGameInstance = Cast<UNetGameInstance>(GetGameInstance());
	if (NetGameInstance) {
		if (MyCharacter) {
			MyCharacter->Server_MakeInvisible(NULL, false);
		}
		if (GetPlayerState()) {
			GetPlayerState()->Destroy();

			//FTimerHandle Handle;
			//FTimerDelegate Delegate = FTimerDelegate::CreateUObject(MyPlayerState, &AConceptPlayerState::DestroyPlayerState);
			//GetWorldTimerManager().SetTimer(Handle, Delegate, 0.1, false);
		}

		//FTimerHandle Handle;
		//FTimerDelegate Delegate = FTimerDelegate::CreateUObject(NetGameInstance, &UNetGameInstance::DestroySessionAndLeaveGame);
		//GetWorldTimerManager().SetTimer(Handle, Delegate, 0.2, false);
		NetGameInstance->DestroySessionAndLeaveGame();
	}
}


void AConceptPlayerController::KillEventToHUD(FString KillerGameTag, FString VictimGameTag, FString KillerNameAbility, int32 KillerTeam, int32 VictimTeam) {
	UUW_HUD* Widget = Cast<UUW_HUD>(InGameHUD);
	if (Widget) {
		Widget->KillEvent(KillerGameTag, VictimGameTag, KillerNameAbility, KillerTeam, VictimTeam);
	}

	//if (InGameHUD && IsLocalController()) {
	//	UFunction* function = InGameHUD->FindFunction("SomeOneIsKilled");
	//	if (function)
	//	{
	//		InGameHUD->ProcessEvent(function, &Data);
	//	}
	//}
}

void AConceptPlayerController::InputManager_Implementation(EControllerInput ControllerInput) {
	if (InputComponent == NULL){ return; }

	InitState();
	InputComponent->ClearActionBindings();
	if (MyCharacter && MyCharacter->InputComponent) {
		MyCharacter->InputComponent->ClearActionBindings();
	}

	switch (ControllerInput)
	{
	case EControllerInput::RE_STANDARD_INPUT:{
		BindKeysToChangeAbility();
		BindKeysToAttack();
		BindKeysToShowStatistics();
		BindKeysToMoveCameraFree();
		BindKeysToMoveCharacter();
		BindGeneralKeys();
	}
											 break;
	case EControllerInput::RE_SPECTATOR:{
		BindGeneralKeys();
	}
											 break;
	case EControllerInput::RE_MENU:{
		BindGeneralKeys();
		BindKeysPauseMenu();
	}
										break;
	default:
		break;
	}

	InputComponent->BindAction("AnyGamepad", IE_Pressed, this, &AConceptPlayerController::OnGamepadPressed);
	InputComponent->BindAction("AnyKeyboardCustom", IE_Pressed, this, &AConceptPlayerController::OnKeyboardPressed);
	InputComponent->BindAxis("Mouse", this, &AConceptPlayerController::MouseMovement);
	this->ControllerInput = ControllerInput;
}

bool AConceptPlayerController::InputManager_Validate(EControllerInput ControllerInput){
	return true;
}


void AConceptPlayerController::ChangeResolutionTo(const FString &ScreenResolutionCommand) {
	this->ConsoleCommand(ScreenResolutionCommand);
	CalculateViewportSize();
}

void AConceptPlayerController::OnTABPressed(){
		HUDManager(EHUDStateEnum::RE_STATISTICS);
}

void AConceptPlayerController::OnTABReleased(){
		HUDManager(EHUDStateEnum::RE_INGAME_HUD);
}

void AConceptPlayerController::OnDamageReceived_Implementation(APawn* CharacterDamager){
	//if (!DamagedHUD) {
	//	HUDManager(EHUDStateEnum::RE_DAMAGE);
	//}else{
	//	UFunction* function = DamagedHUD->FindFunction("Hit");
	//	DamagedHUD->ProcessEvent(function, NULL);
	//}

	if (IsLocalPlayerController() && DamageIndicatorClass) {
		UUW_HUD* DamageIndicator = CreateWidget<UUW_HUD>(this, DamageIndicatorClass);
		if (!DamageIndicator) { return; }
		DamageIndicator->InitDamageIndicator(CharacterDamager);
		//DamageIndicator->AddToViewport(1000);
		AddWidgetToViewport(DamageIndicator, 1000, true);
		DamageIndicator->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

bool AConceptPlayerController::OnDamageReceived_Validate(APawn* CharacterDamager) { return true; }

FRotator AConceptPlayerController::CalculateRotToCursorPosition(FVector StartLocationParam, float UpperAngleLimit, float LowerAngleLimit) {
	if (GetWorld()){

		FRotator TargetRotation;
		FVector WorldLocation, WorldDirection, EndLocation, DirectionVector2;
		DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		EndLocation = WorldLocation + (WorldDirection * (WorldLocation.X / WorldDirection.X)) * -1;

		

		DirectionVector2 = EndLocation - StartLocationParam;



		float angle2 = atan2(DirectionVector2.Z, -DirectionVector2.Y) * 180 / PI;
		
		TargetRotation.Roll = angle2;

		//Gamepad
		if (bIsUsingGamepad) {
			//DirectionVector2 = MyCharacter->AimingPadVector - StartLocationParam;

			

			angle2 = atan2(MyCharacter->AimingPadVector.Z, -MyCharacter->AimingPadVector.Y) * 180 / PI;

			TargetRotation.Roll = angle2 - MyCharacter->CurrentFloorOrientation.Roll;

			FVector_NetQuantize10 Start = StartLocationParam;

		}

		return TargetRotation;
	}

	return FRotator(0, 0, 0);
}

FVector AConceptPlayerController::CalculateCursorPosition()
{
	if (GetWorld()) {

		FRotator TargetRotation;
		FVector WorldLocation, WorldDirection, EndLocation, DirectionVector2;
		DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		EndLocation = WorldLocation + (WorldDirection * (WorldLocation.X / WorldDirection.X)) * -1;

		return EndLocation;
	}

	return FVector(0, 0, 0);
}

FVector2D AConceptPlayerController::CalculateAnimationAimPoint(FVector StartLocationParam, FRotator FloorRotation) {
	FVector2D VectorToReturn = FVector2D(0.0f,0.0f);

	FVector WorldPosition, WorldDirection;
	float XMouse, YMouse;
	GetMousePosition(XMouse, YMouse);

	DeprojectScreenPositionToWorld(XMouse, YMouse, WorldPosition, WorldDirection);

	FVector EndLocation = WorldPosition + (WorldDirection * (WorldPosition.X / WorldDirection.X)) * -1;

	EndLocation = EndLocation - StartLocationParam;
	EndLocation = FloorRotation.RotateVector(EndLocation);

	VectorToReturn.X = -EndLocation.Y;
	VectorToReturn.Y = EndLocation.Z;


	if (bIsUsingGamepad) {
		FVector2D CharacterLocation;
		ProjectWorldLocationToScreen(StartLocationParam, CharacterLocation, false);
		FVector2D Vector = FVector2D(MyCharacter->AimingPadVector.Y, -MyCharacter->AimingPadVector.Z);
		Vector *= 1000;

		VectorToReturn = CharacterLocation - Vector;
	}


	return VectorToReturn;
}

//FVector2D AConceptPlayerController::GetNativeResolution()
//{
//	FVector2D ToReturn;
//
//	FDisplayMetrics Metrics;
//	FDisplayMetrics::GetDisplayMetrics(Metrics);
//	ToReturn = FVector2D(Metrics.PrimaryDisplayWidth, Metrics.PrimaryDisplayHeight);
//
//	return ToReturn;
//}


//HUD FUNCTIONS
void AConceptPlayerController::HUDManager_Implementation(EHUDStateEnum NextHUDState){

	if (NextHUDState != EHUDStateEnum::RE_DAMAGE && HUDState == NextHUDState){ return; }
	if (NextHUDState != EHUDStateEnum::RE_DAMAGE){
		RemoveAllHudElements();
	}
	
	switch (NextHUDState){
		case EHUDStateEnum::RE_INGAME_HUD:
			InitInGameHUD();
			break;
		case EHUDStateEnum::RE_PLAYER_LOADER:
			InitPlayerLoaderHUD();
			break;
		case EHUDStateEnum::RE_STATISTICS:
			InitInGameStatisticsHUD();
			break;
		case EHUDStateEnum::RE_DEATHSTATISTICS:
			InitInGameStatisticsHUD();
			//InitDeathStatisticsHUD();
			break;
		case EHUDStateEnum::RE_FINALSTATISTICS:
			ShowResults();
			break;
		case EHUDStateEnum::RE_RESULTS:
			ShowResults();
			break;
		case EHUDStateEnum::RE_DAMAGE:
			InitInGameDamageHUD();
			break;
		case EHUDStateEnum::RE_STEALTH:
			InitInGameStealthHUD();
			break;
		default:
			break;
	}
	if(NextHUDState != EHUDStateEnum::RE_DAMAGE){
		HUDState = NextHUDState;
	}
	
}

void AConceptPlayerController::RemoveAllHudElements(){
	if (PlayerLoader){RemovePlayerLoaderHUD();}
	if (InGameHUD) {RemoveInGameHUD();}
	if (InGameStatistics) {RemoveInGameStatisticsHUD();}
	if (DeathStatistics){RemoveDeathStatisticsHUD();}
	if (FinalStatistics) {RemoveFinalStatisticsHUD();}
	

}

void AConceptPlayerController::InitWidget(TSubclassOf<class UUserWidget> WidgetClass, UUserWidget* &Widget, float ZOrder, bool bShouldAddToScreen){
	if (IsLocalPlayerController() && WidgetClass) {
		if (!Widget) {
			Widget = CreateWidget<UUserWidget>(this, WidgetClass);
			if (!Widget) { return; }
			//Widget->AddToViewport(ZOrder);
			//Widget->AddToPlayerScreen(ZOrder);
			//AddWidgetToViewport(Widget, ZOrder, bShouldAddToScreen);

			if (bShouldAddToScreen) {
				Widget->AddToPlayerScreen(ZOrder);
			}
			else {
				Widget->AddToViewport(ZOrder);
			}

		}
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AConceptPlayerController::AddWidgetToViewport_Implementation(UUserWidget* Widget, float ZOrder, bool bShouldAddToScreen){
	if (bShouldAddToScreen) {
		Widget->AddToPlayerScreen(ZOrder);
	}
	else {
		Widget->AddToViewport(ZOrder);
	}
	
}

void AConceptPlayerController::RemoveWidget(UUserWidget* &Widget){
	if (IsLocalPlayerController() && Widget) {
		Widget->RemoveFromViewport();
	}
}


void AConceptPlayerController::InitPauseMenu_Implementation() {
	InitWidget(PauseMenuClass, PauseMenuHUD);
	CurrentMouseCursor = EMouseCursor::Default;
}

void AConceptPlayerController::RemovePauseMenu_Implementation() {
	if (IsLocalPlayerController() && PauseMenuHUD) {
		//UFunction* function = PauseMenuHUD->FindFunction("RemoveWidget");
		//PauseMenuHUD->ProcessEvent(function, NULL);
		//UpgradesHUD->RemoveFromViewport();
		PauseMenuHUD->RemoveFromParent();
	}
	PauseMenuHUD = NULL;
	bShowMouseCursor = true;
	CurrentMouseCursor = EMouseCursor::SlashedCircle;
}


void AConceptPlayerController::InitPlayerLoaderHUD_Implementation() {
	InitWidget(PlayerLoaderClass, PlayerLoader, 100.0f, false);
	CurrentMouseCursor = EMouseCursor::Default;
}

void AConceptPlayerController::RemovePlayerLoaderHUD_Implementation(){
	//RemoveWidget(PlayerLoader);
	if (IsLocalPlayerController() && PlayerLoader) {
		UFunction* function = PlayerLoader->FindFunction("RemoveWidget");
		PlayerLoader->ProcessEvent(function, NULL);
	}
	PlayerLoader = NULL;
}

void AConceptPlayerController::InitInGameHUD_Implementation() {
	InitWidget(InGameHUDClass, InGameHUD);
	if (InGameHUD) {
		InGameHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void AConceptPlayerController::RemoveInGameHUD_Implementation() {
	//RemoveWidget(InGameHUD);
	if (IsLocalPlayerController() && InGameHUD) {
		//UFunction* function = InGameHUD->FindFunction("RemoveWidget");
		//InGameHUD->ProcessEvent(function, NULL);
		InGameHUD->SetVisibility(ESlateVisibility::Hidden);
	}
	
	//InGameHUD = NULL;
}

void AConceptPlayerController::InitInGameStatisticsHUD_Implementation() {
	InitWidget(InGameStatisticsClass, InGameStatistics);
	CurrentMouseCursor = EMouseCursor::SlashedCircle;
}

void AConceptPlayerController::InitDeathStatisticsHUD_Implementation() {
	InitWidget(DeathStatisticsClass, DeathStatistics);
	CurrentMouseCursor = EMouseCursor::Default;
}

void AConceptPlayerController::InitFinalStatisticsHUD_Implementation(int32 Winner) {
	InitWidget(FinalStatisticsClass, FinalStatistics);
	CurrentMouseCursor = EMouseCursor::Default;


	UFunction* function = FinalStatistics->FindFunction("SetResult");
	FinalStatistics->ProcessEvent(function, &Winner);
}

void AConceptPlayerController::InitInGameDamageHUD_Implementation() {
	InitWidget(DamageHUDClass, DamagedHUD, -1000);
}

void AConceptPlayerController::InitInGameStealthHUD_Implementation() {
	InitWidget(StealthHUDClass, InvisibleHUD);
}

void AConceptPlayerController::InitInGameHealthHUD_Implementation() {
	InitWidget(HealthHUDClass, HealthHUD);
}

void AConceptPlayerController::RemoveInGameStealthHUD_Implementation() {
	InvisibleHUD->SetVisibility(ESlateVisibility::Collapsed);
	RemoveWidget(InvisibleHUD);

	InvisibleHUD = NULL;
}

void AConceptPlayerController::RemoveDamageHUD_Implementation(){
	DamagedHUD->SetVisibility(ESlateVisibility::Collapsed);
	RemoveWidget(DamagedHUD);
}

void AConceptPlayerController::RemoveHealthHUD_Implementation() {
	HealthHUD->SetVisibility(ESlateVisibility::Collapsed);
	RemoveWidget(HealthHUD);
}

void AConceptPlayerController::RemoveInGameStatisticsHUD_Implementation() {
	//RemoveWidget(InGameStatistics);
	if (IsLocalPlayerController() && InGameStatistics) {
		UFunction* function = InGameStatistics->FindFunction("RemoveWidget");
		InGameStatistics->ProcessEvent(function, NULL);
	}

	InGameStatistics = NULL;
	
}

void AConceptPlayerController::RemoveDeathStatisticsHUD_Implementation() {
	if (IsLocalPlayerController() && DeathStatistics) {
		UFunction* function = DeathStatistics->FindFunction("RemoveWidget");
		DeathStatistics->ProcessEvent(function, NULL);
	}
	RemoveWidget(DeathStatistics);
	DeathStatistics = NULL;
	CurrentMouseCursor = EMouseCursor::SlashedCircle;
}

void AConceptPlayerController::RemoveFinalStatisticsHUD_Implementation() {
	//RemoveWidget(InGameStatistics);
	if (IsLocalPlayerController() && FinalStatistics) {
		UFunction* function = FinalStatistics->FindFunction("RemoveWidget");
		FinalStatistics->ProcessEvent(function, NULL);
	}

	FinalStatistics = NULL;
}

//CAMERA FUNCTIONS
void AConceptPlayerController::ClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation) {
	SetInitialLocationAndRotation(CameraLocation, CameraRotation);
	AdjustClientSetSpectatorCamera(CameraLocation, CameraRotation);
	//SetViewTarget(this);
	//Possess(GetSpectatorPawn());
	if (PlayerState)
	{
		PlayerState->bIsSpectator = true;
		ChangeState(NAME_Spectating);
		ClientGotoState(NAME_Spectating);
	}
}
void AConceptPlayerController::AdjustClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation) {
	SetInitialLocationAndRotation(CameraLocation, CameraRotation);
}

void AConceptPlayerController::CalculateViewportSize_Implementation() {
	ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	CenterOfViewport = ViewportSize / 2;
	
}


void AConceptPlayerController::PauseMenuManager(){
	if(PauseMenuHUD == NULL){
		InitPauseMenu();
		AddWidget(PauseMenuHUD);
		InputManager(EControllerInput::RE_MENU);
	}
	else{
		RemoveWidget(PauseMenuHUD);
		RemovePauseMenu();
		InputManager(EControllerInput::RE_STANDARD_INPUT);
	}
}

void AConceptPlayerController::SetMousePositionX(float LocationX)
{
// 	AAbility* CurrentAbility = MyCharacter->AbilityInUse;
// 	if (FMath::Abs(LocationX) > 0.1 && CurrentAbility){
// 
// 		if (IsLocalController()){
// 
// 			MyCharacter->CalculateLinearRotationToAim(CalculateRotToCursorPosition(CurrentAbility->GetPointToCalculateRotationToCursor(0), 10, 0));
// 		}
// 	}
	//if (FMath::Abs(LocationX) > 0.25){
	//	FViewport* v = CastChecked<ULocalPlayer>(this->Player)->ViewportClient->Viewport;
	//	VirtualMousePosition.X += LocationX;
	//	v->SetMouse((int32)VirtualMousePosition.X, (int32)VirtualMousePosition.Y);
	//}
}

void AConceptPlayerController::SetMousePositionY(float LocationY)
{
// 	AAbility* CurrentAbility = MyCharacter->AbilityInUse;
// 	if (FMath::Abs(LocationY) > 0.1 && CurrentAbility){
// 
// 		if (IsLocalController()){
// 
// 			MyCharacter->CalculateLinearRotationToAim(CalculateRotToCursorPosition(CurrentAbility->GetPointToCalculateRotationToCursor(0), 10, 0));
// 		}
// 	}
	//if (FMath::Abs(LocationY) > 0.25){
	//	FViewport* v = CastChecked<ULocalPlayer>(this->Player)->ViewportClient->Viewport;
	//	VirtualMousePosition.Y += LocationY;
	//	v->SetMouse((int32)VirtualMousePosition.X, (int32)VirtualMousePosition.Y);
	//}
}

void AConceptPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AConceptPlayerController, HUDState);
	DOREPLIFETIME(AConceptPlayerController, MyPlayerState);
	DOREPLIFETIME(AConceptPlayerController, MyCharacter);

}

void AConceptPlayerController::CalculateWorldPositionOfMouse(){
	FVector EarlyMouseLocation;
	DeprojectMousePositionToWorld(EarlyMouseLocation, DirectionMouse);
	DirectionMouse = ClampVector(-1 * DirectionMouse, FVector(1, -0.7, -0.7), FVector(0, 0.7, 0.7));
	Mouse3DLocation = (EarlyMouseLocation + (DirectionMouse * (EarlyMouseLocation.X / DirectionMouse.X)));
}

void AConceptPlayerController::CalculateMouse2DLocation(){
	float MouseX, MouseY;
	GetMousePosition(MouseX, MouseY);
	Mouse2DLocation.X = MouseX;
	Mouse2DLocation.Y = MouseY;
}

void AConceptPlayerController::ShowResults_Implementation(){
	MyGameMode = Cast<AConceptGameMode>(GetWorld()->GetAuthGameMode());
	MyPlayerState = Cast<AConceptPlayerState>(PlayerState);

	int32 Winner = MyGameMode->GetWinnerTeam();
	if (Winner == -1){ 
		InitFinalStatisticsHUD(DRAW); 
	}
	else if (Winner == MyPlayerState->Team) {
		InitFinalStatisticsHUD(VICTORY);
	}
	else {
		InitFinalStatisticsHUD(DEFEAT);
	}
}

void AConceptPlayerController::MakeTeleport_Implementation() {
	MyPlayerState->StatsAddTeleport();
}

void AConceptPlayerController::ShowInGameHUD_Implementation(){

}

void AConceptPlayerController::BindKeysToShowStatistics(){
	InputComponent->BindAction("Statistics", IE_Pressed, this, &AConceptPlayerController::OnTABPressed);
	InputComponent->BindAction("Statistics", IE_Released, this, &AConceptPlayerController::OnTABReleased);
}


void AConceptPlayerController::BindKeysToChangeAbility(){
	if (InputComponent && MyCharacter){
		InputComponent->BindAction("Ability1", IE_Pressed, MyCharacter, &AConceptCharacter::AbilityTestInUse<0>);
		InputComponent->BindAction("Ability2", IE_Pressed, MyCharacter, &AConceptCharacter::AbilityTestInUse<1>);
		InputComponent->BindAction("Ability3", IE_Pressed, MyCharacter, &AConceptCharacter::AbilityTestInUse<2>);
		InputComponent->BindAction("Ability4", IE_Pressed, MyCharacter, &AConceptCharacter::AbilityTestInUse<3>);//Q
		InputComponent->BindAction("Ability5", IE_Pressed, MyCharacter, &AConceptCharacter::AbilityTestInUse<4>);//E
	}

}

void AConceptPlayerController::BindKeysToAttack(){
	if (InputComponent && MyCharacter){
		InputComponent->BindAction("MainAbility", IE_Pressed, MyCharacter, &AConceptCharacter::FireMainAbility);
		InputComponent->BindAction("MainAbility", IE_Released, MyCharacter, &AConceptCharacter::StopMainAbility);
		InputComponent->BindAction("SpecialAbility", IE_Pressed, MyCharacter, &AConceptCharacter::PerformSpecialAbility);
		InputComponent->BindAction("Shield", IE_Pressed, MyCharacter, &AConceptCharacter::EnableShieldServer);
		InputComponent->BindAction("Shield", IE_Released, MyCharacter, &AConceptCharacter::DisableShieldServer);
		InputComponent->BindAction("AutoAim", IE_Pressed, MyCharacter, &AConceptCharacter::ToggleAutoAim);
	}
}


void AConceptPlayerController::BindKeysToMoveCharacter(){
	if (InputComponent && MyCharacter){
		InputComponent->BindAction("Sprint", IE_Pressed, MyCharacter, &AConceptCharacter::OnSprintPressed);
		InputComponent->BindAction("Sprint", IE_Released, MyCharacter, &AConceptCharacter::OnSprintReleased);
		InputComponent->BindAction("Jump", IE_Pressed, MyCharacter, &AConceptCharacter::Jump);
		InputComponent->BindAction("InverseDash", IE_Pressed, MyCharacter, &AConceptCharacter::InverseDash);
		//InputComponent->BindAction("Crouch", IE_Pressed, MyCharacter, &AConceptCharacter::OnCrouchPressed);
		//InputComponent->BindAction("Crouch", IE_Released, MyCharacter, &AConceptCharacter::OnCrouchReleased);
		InputComponent->BindAxis("TurnX", MyCharacter, &AConceptCharacter::AimAxisX);
		InputComponent->BindAxis("TurnY", MyCharacter, &AConceptCharacter::AimAxisY);
		InputComponent->BindAxis("MoveRight", MyCharacter, &AConceptCharacter::MoveRight);
		InputComponent->BindAxis("MoveUp", MyCharacter, &AConceptCharacter::MoveUp);
		InputComponent->BindAction("Reload", IE_Released, MyCharacter, &AConceptCharacter::Reload);
		InputComponent->BindAction("Roll", IE_Pressed, MyCharacter, &AConceptCharacter::MakeRoll);
		InputComponent->BindAction("Teleport", IE_Pressed, this, &AConceptPlayerController::MakeTeleport);
	}
}

void AConceptPlayerController::BindKeysPauseMenu(){
	if (InputComponent && MyCharacter)
	{	
		for (int32 i = 0; i < EnabledWidgets.Num(); i++) {
			InputComponent->BindAction("Accept_General", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnCrossPressed);
			InputComponent->BindAction("Back_General", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnCirclePressed);
			InputComponent->BindAction("Up_General", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnUpArrowPressed);
			InputComponent->BindAction("Down_General", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnDownArrowPressed);
			InputComponent->BindAction("Right_General", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnRightPressed);
			InputComponent->BindAction("Left_General", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnLeftArrowPressed);
			InputComponent->BindAction("SwitchCategoryRight_Settings", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnR1Pressed);
			InputComponent->BindAction("SwitchCategoryLeft_Settings", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnL1Pressed);
			InputComponent->BindAction("Apply_Settings", IE_Pressed, Cast<UUW_HUD>(EnabledWidgets[i]), &UUW_HUD::OnSquarePressed);
			

			
		}
	}
}

void AConceptPlayerController::BindGeneralKeys(){
	if (InputComponent && MyCharacter) {
		InputComponent->BindAction("Exit", IE_Pressed, this, &AConceptPlayerController::PauseMenuManager);
        InputComponent->BindAction("DemoRec", IE_Pressed, this, &AConceptPlayerController::DemoRec);
        InputComponent->BindAction("DemoStop", IE_Pressed, this, &AConceptPlayerController::DemoStop);
        InputComponent->BindAction("DemoPlay", IE_Pressed, this, &AConceptPlayerController::DemoPlay);

	}
}

void AConceptPlayerController::BindKeysToMoveCameraFree(){
	if (InputComponent && MyCharacter){
		//InputComponent->BindAction("FreeCamera", IE_Pressed, MyCharacter, &AConceptCharacter::MoveCameraWithFreedomPressed);
		//InputComponent->BindAction("FreeCamera", IE_Released, MyCharacter, &AConceptCharacter::MoveCameraWithFreedomReleased);
	}
}

void AConceptPlayerController::DemoRec(){
    UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
    if(GI){
        TArray < FString >  AdditionalOptions;
        GI->StartRecordingReplay("HIVE_0", "HIVE_0", AdditionalOptions);
    }
}

void AConceptPlayerController::DemoStop(){
    UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
    if(GI){
        GI->StopRecordingReplay();
    }
}

void AConceptPlayerController::DemoPlay(){
    UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
    if(GI){
        UWorld* world = GetWorld();
        if(world){
            TArray < FString >  AdditionalOptions2;
            GI->PlayReplay("HIVE_0", world, AdditionalOptions2);
        }
    }
}

bool AConceptPlayerController::ClientSetSpectatorCamera_Validate(FVector CameraLocation, FRotator CameraRotation) {
	return true;
}

bool AConceptPlayerController::HUDManager_Validate(EHUDStateEnum NextHUDState){
	return true;
}

AConceptCharacter* AConceptPlayerController::GetConceptCharacter()
{
	return MyCharacter;
}

AConceptPlayerState* AConceptPlayerController::GetPlayerState()
{
	return MyPlayerState;
}

void AConceptPlayerController::PlayForceFeedback(float Damage) {
	FLatentActionInfo latentInfo;
	latentInfo.CallbackTarget = this;
	//MyCharacter->MyPlayerController->ClientPlayForceFeedback(DamageForceFeedback, false, "None");
	PlayDynamicForceFeedback(Damage / 50, Damage / 50, true, true, true, true, EDynamicForceFeedbackAction::Start, latentInfo);
}

bool AConceptPlayerController::ShowResults_Validate() {return true;}

