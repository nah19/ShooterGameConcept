// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "ConceptCharacter.h"
#include "Ability.h"
#include "Projectile.h"
#include "ConceptGameMode.h"
#include "StaticFunctionLibrary.h"
#include "DB_Character.h"
#include "HiveGravityZone.h"
#include "ConceptDamageType.h"
#include "A_BaseWeapon.h"
#include "NetGameInstance.h"
#include "D_PoisonDamage.h"
#include "D_BurnDamage.h"
#include "D_ConstrictorDamage.h"
#include "P_ZeroGravityZone.h"
#include "ConceptStats.h"
#include "CustomCharacterMovementComponent.h"



AConceptCharacter::AConceptCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {

	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->TargetArmLength = 2000.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	SideViewCameraComponent->FieldOfView = 100;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RadialForceComponent->RelativeRotation = FRotator(0.f, 0.f, 0.f);


	// Configure character movement
	UCharacterMovementComponent *MovementComponent = GetCharacterMovement();
	//MovementComponent->bOrientRotationToMovement = true; // Face in the direction we are moving..

	NameOfCharacter = "EMPTY NAME";
	MapPawnOrientations.SetNumUninitialized(2);
	OrientedTo = EOrientationToEnum::RE_FORWARD;
	CharacterScreenBase2DLocation = FVector2D(0, 0);
	TimeSinceLastMovement = 0;
	TurnRateOfCharacter = 5;
	bReplicates = true;
	bReplicateMovement = true;
	SetReplicates(true);
	ShouldTurn = false;
	IsRegenerating = false;
	CameraSpeedParam = 1.8f;
	FreeCameraSpeedParam = 3500;
	FiringType = -1;
	SprintKeyPressed = false;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanWalk = true;
	IndexOfAbilityInUse = 0;
	bOtherHasReceivedDamage = false;
	IsChangingAbility = false;
	TimeToInactive = 100;
	AbleToMove = true;
	bAbleToShoot = false;
	bUseGravity = true;
	bAbleToDash = true;
	CanReceiveDamage = false;
	TimeToReceiveDamage = 5.0f;
	IncrementOfWheel = 0;
	VelocityMultiplierOnUse = 1.0f;
	LastMovementY = 0.0f;
	LastMovementX = 0.0f;
	LastMovementYLerped = 0.0f;
	LastMovementXLerped = 0.0f;
	TimesSinceLastDamage.SetNumUninitialized(0);
	WeaponToChange.SetNumUninitialized(0);
	bIsShieldActivated = false;

	ZeroGravitySpeed = FVector::ZeroVector;
	CharacterState = ECharacterStateEnum::CharacterState::RE_NORMAL;
	
	
	ResetStatus();

}

void AConceptCharacter::FillCharacterVariablesWithData(UPARAM(ref) FGenericCharacterData& DataParam) {
	LifeMax = DataParam.LifeMax;
	Life = LifeMax;
	Armor = DataParam.Armor;
	ShieldMax = DataParam.ShieldMax;
	Shield = ShieldMax;
	Speed = DataParam.SpeedMax;
	SprintSpeed = Speed * 2;
	SetCharacterSpeed(Speed);
	JumpPower = DataParam.JumpPower;
	SetCharacterJump(JumpPower);
	LifeReg = DataParam.LifeReg;
	ShieldReg = DataParam.ShieldReg;
	StaminaReg = DataParam.StaminaReg;
	StaminaMax = DataParam.StaminaMax;
	StaminaExp = DataParam.StaminaExp;
	Stamina = StaminaMax;
}

void AConceptCharacter::InitCharacterData(){
}

void AConceptCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	UStaticFunctionLibrary::GetCharacterDataFromSelected(Data, Data.CharacterSelected);
	FillCharacterVariablesWithData(Data);
	InitState();
}

void AConceptCharacter::BeginPlay() {
	Super::BeginPlay();
	UStaticFunctionLibrary::GetCharacterDataFromSelected(Data, Data.CharacterSelected);
	FillCharacterVariablesWithData(Data);

	InitState();
	InitMouse();
	InitGameAttributes();
	MyDamageBehavior = NewObject<UDB_Character>();
	MyDamageBehavior->Initialize((AActor*)this);

	if (InversePlayer) {
		InitGravity();
	}

	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::SetCanReceiveDamage, true);
	GetWorldTimerManager().SetTimer(Handle, Delegate, TimeToReceiveDamage, false);

}

void AConceptCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	InitState();
}

void AConceptCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (MyPlayerController) {
		float MouseX = 0;
		float MouseY = 0;
		MyPlayerController->GetMousePosition(MouseX, MouseY);
		FVector2D CurrentMousePosition = FVector2D(MouseX, MouseY);

		if (CurrentMousePosition != LastMousePosition) {
			//MyPlayerController->bIsUsingGamepad = false;
        }
        if(!MyPlayerController->bIsUsingGamepad){
            //StartMovingAimTo();
        } else if (bGameAxisInactive){
            GamePadAiming(0, 0);
        }
	}
    
    
	LerpForLinearRotation();
	CameraManager();
	UpdateStatus(DeltaSeconds);


	switch (CharacterState) {
	case ECharacterStateEnum::CharacterState::RE_NORMAL:
		GravityRotatorCalculator(DeltaSeconds);
		break;
	case ECharacterStateEnum::CharacterState::RE_ZEROGRAVITY:
		ZeroGravityRotatorCalculator(DeltaSeconds);
		break;
	default:
		GravityRotatorCalculator(DeltaSeconds);
		break;
	}

	for (int32 i = 0; i < TimesSinceLastDamage.Num(); i++) {
		TimesSinceLastDamage[i] -= DeltaSeconds;
		if (TimesSinceLastDamage[i] <= 0) {
			TimesSinceLastDamage.RemoveAt(i);
			Damagers.RemoveAt(i);
		}
	}

	if (MyPlayerState) {
		if (GetCharacterMovement()->IsFalling()) { MyPlayerState->StatsAddTimeFlying(DeltaSeconds); }
		else if (IsSprinting) { MyPlayerState->StatsAddTimeSprinting(DeltaSeconds); }
		else { MyPlayerState->StatsAddTimeWalking(DeltaSeconds); }
	}

}

void AConceptCharacter::InitState() {
	InitOwnState();
	InitStrangerState();
}

void AConceptCharacter::InitOwnState() {
	if (!MyPlayerController) { MyPlayerController = Cast<AConceptPlayerController>(GetController()); }
	if (!MyPlayerState) { 
		MyPlayerState = Cast<AConceptPlayerState>(PlayerState); 
		if(Cast<APS_Survival>(MyPlayerState)){
			StaminaReg = StaminaReg * 0.05f;
		}
	}
	if (!MyGameMode) { MyGameMode = Cast<AConceptGameMode>(GetWorld()->GetAuthGameMode()); }
	if (!MyGameState) { MyGameState = Cast<AConceptGameState>(GetWorld()->GetGameState()); }
}

void AConceptCharacter::InitStrangerState() {
	if (MyPlayerController) { 
		MyPlayerController->SetMyCharacter(this); 
		MyPlayerController->MyCharacter = this;
		MyPlayerController->InputManager(EControllerInput::RE_STANDARD_INPUT);
		MyPlayerController->HUDManager(EHUDStateEnum::RE_INGAME_HUD);
	}
	if (MyPlayerState) { 
		MyPlayerState->SetMyCharacter(this); 
		if (MyPlayerState->Team == 1) { InitGravity(); }
		MyPlayerState->ServerInitAbilities(Ab1, Ab2, Ab3, Ab4, Ab5);
	}
}

void AConceptCharacter::InitGameAttributes() {
    if(!MyGameState) return;
	if (MyGameState->InProgressState == EInProgressStateEnum::RE_WAITINGTOBEINPROGRESS || MyGameState->InProgressState == EInProgressStateEnum::RE_EARLYSTAGE) {
		CustomTurnOff();
	}

	PlaySound_Server(BreathCue, BreathSoundComponent, true);
}

void AConceptCharacter::InitMouse() {
	ShouldTurn = true;
	if(MyPlayerController && MyPlayerState) {
		UpdatesBasedInMouseStats();
		StartMovingAimTo();
	}
}

void AConceptCharacter::InitGravity() {
	if(GetCharacterMovement()){
		GetCustomCharacterMovement()->SetGravityDirection(FVector(0, 0, 1));
		//GetCharacterMovement()->SetGravityDirection(FVector(0, 0, 1));
		MapPawnOrientations[0] = FRotator(0, 90, 180);
		MapPawnOrientations[1] = FRotator(0, -90, 180);
		CurrentFloorOrientation = FRotator(0, 0, 180);
	}
}

void AConceptCharacter::SetMyPlayerState_Implementation(AConceptPlayerState* Char) { MyPlayerState = Char; }
void AConceptCharacter::SetMyPlayerController_Implementation(AConceptPlayerController* PController) { MyPlayerController = PController; }
void AConceptCharacter::SetMyGameMode_Implementation(AConceptGameMode* Game) { MyGameMode = Game; }
void AConceptCharacter::SetMyGameState_Implementation(AConceptGameState* State) { MyGameState = State; }
bool AConceptCharacter::SetMyPlayerState_Validate(AConceptPlayerState* Char) { return true; }
bool AConceptCharacter::SetMyPlayerController_Validate(AConceptPlayerController* PController) { return true; }
bool AConceptCharacter::SetMyGameMode_Validate(AConceptGameMode* Char) { return true; }
bool AConceptCharacter::SetMyGameState_Validate(AConceptGameState* Char) { return true; }

void AConceptCharacter::ActivateZeroGravity()
{
	if (CharacterState == ECharacterStateEnum::CharacterState::RE_ZEROGRAVITY)
		return;

	CharacterState = ECharacterStateEnum::CharacterState::RE_ZEROGRAVITY;
	bAbleToDash = false;
	bUseGravity = false;
	bAbleToShoot = false;
	ZeroGravitySpeed = GetMovementComponent()->Velocity / 2.0f;
	PreviousZeroGRotation = Gravity.Rotation();
	PreviousZeroGRotation.Pitch += 90.0f;
	GetMovementComponent()->Velocity = ZeroGravitySpeed;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	PreviousGravity = Gravity;
	ZeroGravityRotation = FVector::RightVector.Rotation();
	VelocityMagnitude = GetMovementComponent()->Velocity.Size();
}

void AConceptCharacter::DeactivateZeroGravity()
{
	CharacterState = ECharacterStateEnum::CharacterState::RE_NORMAL;
	bUseGravity = true;
	bAbleToDash = true;
	bAbleToShoot = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	GetCustomCharacterMovement()->SetGravityDirection(Gravity);
}

void AConceptCharacter::TemporalZeroGravity(float Time)
{
	ActivateZeroGravity();

	/*FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::DeactivateZeroGravity);
	GetWorldTimerManager().SetTimer(Handle, Delegate, Time, false);*/

}





void AConceptCharacter::OnCrouchPressed(){
	if (!GetCharacterMovement()->IsFalling()){
		Crouch(true);
	}
}

void AConceptCharacter::OnCrouchReleased(){
	UnCrouch(true);
}

void AConceptCharacter::PlaySound(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol) {
	if(IsAttached && Component){
		//Component = UGameplayStatics::PlaySoundAttached(Sound, GetCapsuleComponent(),NAME_None, FVector(ForceInit), EAttachLocation::KeepRelativeOffset, false, Vol);
	}
	else{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, GetActorLocation(),FRotator::ZeroRotator,Vol);
	}
}

void AConceptCharacter::StopSound(UAudioComponent* Component) {
	if (Component) {
		Component->Stop();
	}
}

void AConceptCharacter::PlaySound_Server_Implementation(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol) {
	PlaySound_Multicast(Sound, Component, IsAttached, Vol);
}

void AConceptCharacter::StopSound_Server_Implementation(UAudioComponent* Component) {
	StopSound_Multicast(Component);
}

void AConceptCharacter::PlaySound_Client_Implementation(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol) {
	PlaySound(Sound, Component, IsAttached, Vol);
}

void AConceptCharacter::StopSound_Client_Implementation(UAudioComponent* Component) {
	StopSound(Component);
}

void AConceptCharacter::PlaySound_Multicast_Implementation(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol) {
	PlaySound(Sound, Component, IsAttached, Vol);
}

void AConceptCharacter::StopSound_Multicast_Implementation(UAudioComponent* Component) {
	StopSound(Component);
}


void AConceptCharacter::Jump(){
	if (Stamina < 5 || !AbleToMove) { return; }
	
	if (bAbleToDash && GetCharacterMovement()->IsFalling() && Stamina > 15) {
		MyPlayerState->StatsAddDash();
		Dash(1);
		return;
	}
	
	if (GetCharacterMovement()->IsFalling()) { return; }
	Stamina = Stamina - 4;
	MyPlayerState->StatsAddJump();
	Super::Jump();
}

void AConceptCharacter::SetAbleToDash(bool Able){
	bAbleToDash = Able;
}

void AConceptCharacter::InverseDash() {
	if (GetCharacterMovement()->IsFalling() && Stamina > 15) {
		Dash(-1);
	}
}

void AConceptCharacter::Dash(int32 DashDirection) {
	if (MyPlayerState /*&& MyPlayerState->SteamStats*/) {
		//MyPlayerState->SteamStats->IncrementLocalStat("Dashes", 1.0);
		UFunction* function = this->FindFunction("DoubleJump");
		this->ProcessEvent(function, &DashDirection);
	}
}

void AConceptCharacter::SetCapsuleCollisionResponseOnServer_Implementation(UPrimitiveComponent* Primitive, ECollisionChannel Channel, ECollisionResponse NewResponse){
	SetCapsuleCollisionResponseOnMulticast(Primitive, Channel, NewResponse);
}

void AConceptCharacter::SetCapsuleCollisionResponseOnMulticast_Implementation(UPrimitiveComponent* Primitive, ECollisionChannel Channel, ECollisionResponse NewResponse){
	Primitive->SetCollisionResponseToChannel(Channel, NewResponse);
}


void AConceptCharacter::SetIsPlayingMontage(bool State){
	IsPlayingMontage = State;
}

void AConceptCharacter::LerpForLinearRotation(){

	if (!MyPlayerController || !MyPlayerController->IsLocalController()) {
		LocalLinearVectorToAim = FMath::Lerp(LocalLinearVectorToAim, LinearVectorToAim, LerpSpeed);
	}

	else{
		float MouseX, MouseY;
		MyPlayerController->GetMousePosition(MouseX, MouseY);
		FVector2D CurrentMousePosition = FVector2D(MouseX, MouseY);

		MyPlayerController->ProjectWorldLocationToScreen(GetActorLocation(), CharacterScreenBase2DLocation);
		MyPlayerController->CalculateMouse2DLocation();

		//CalculateWherePlayerIsLookingTo(MyPlayerController->Mouse2DLocation.X, CharacterScreenBase2DLocation.X);

		if (AbilityInUse) {
			FVector2D AimVector = MyPlayerController->CalculateAnimationAimPoint(AbilityInUse->GetPointToCalculateRotationToCursor(0), CurrentFloorOrientation);
			LocalLinearVectorToAim = AimVector;
			LastRotation = AimVector;
		}
	}
}

void AConceptCharacter::AimAxisX(float MovementX) {
	AxisX = MovementX;
	GamePadAiming(AxisX, AxisY);
	
}

void AConceptCharacter::AimAxisY(float MovementY) {
	AxisY = MovementY;
	GamePadAiming(AxisX, AxisY);
}

void AConceptCharacter::GamePadAiming(float MovementX, float MovementY) {

	float SignoX = (MovementX > 0) ? 1 : -1;
	float SignoY = (MovementY > 0) ? 1 : -1;

	MovementX = FMath::Abs(MovementX);
	MovementY = FMath::Abs(MovementY);

	//if (MovementX >= MovementY) {
	//	MovementX = FMath::Sqrt(FMath::Pow(1.4, 2) - FMath::Pow(MovementY, 2));
	//}
	//else {
	//	MovementY = FMath::Sqrt(FMath::Pow(1.4, 2) - FMath::Pow(MovementX, 2));
	//}



	//if(FMath::Abs(MovementX) < 0.01){
	MovementX = FMath::Pow(MovementX, 0.9);
	//}
		
	//if (FMath::Abs(MovementX) < 0.01) {
	MovementY = FMath::Pow(MovementY, 0.9);
	//}

	MovementX *= SignoX;
	MovementY *= SignoY;


	if (AbilityInUse) {
        if(bAutoAim){

            MovementX = LastMovementX;
            MovementY = LastMovementY;


            
            FVector AutoAimingVector = FVector(0.0f, MovementX, MovementY);
            AutoAimingVector.Normalize();
            AutoAimingVector.Y *= -1344;
            AutoAimingVector.Z *= -756;
            AimVector2D.X = -AutoAimingVector.Y * 1;
            AimVector2D.Y = AutoAimingVector.Z * 1;
            LocalLinearVectorToAim = AimVector2D;
            LastRotation = AimVector2D;
            
            MyPlayerController->ProjectWorldLocationToScreen(GetActorLocation(), CharacterScreenBase2DLocation);
            //CalculateWherePlayerIsLookingTo(AimVector2D.X, CharacterScreenBase2DLocation.X);
            return;
        }
        if (FMath::Abs(MovementX) < 0.1 && FMath::Abs(MovementY) < 0.1) {
            bGameAxisInactive=true;
            if(!MyPlayerController->bIsUsingGamepad){
                return;
            }
            MovementX = LastMovementX;
            MovementY = LastMovementY;
        } else {
            bGameAxisInactive=false;
        }



        //MyPlayerController->bIsUsingGamepad = true;
        FVector AimingVector = FVector(0.0f, MovementX, MovementY);
        if(AimingVector.Size() >= 0.75){
			MovementX = FMath::Lerp(LastMovementX, MovementX, 0.04f);
			MovementY = FMath::Lerp(LastMovementY, MovementY, 0.04f);
            LastMovementX = MovementX;
            LastMovementY = MovementY;
        }

		AimingVector = FVector(0.0f, MovementX, MovementY);
        AimingVector.Normalize();
        AimingVector.Y *= -1344;
        AimingVector.Z *= -756;
		AimingVector.X = 0;
        AimingPadVector = AimingVector;
        FVector LookingAtVector = AimingVector - AbilityInUse->GetPointToCalculateRotationToCursor(0);

        AimVector2D.X = -AimingVector.Y * 1;
        AimVector2D.Y = AimingVector.Z * 1;
        //SetLinearVectorToAim(AimVector2D);
        LocalLinearVectorToAim = AimVector2D;
        LastRotation = AimVector2D;
        
        MyPlayerController->ProjectWorldLocationToScreen(GetActorLocation(), CharacterScreenBase2DLocation);
		//CalculateWherePlayerIsLookingTo(AimVector2D.X, CharacterScreenBase2DLocation.X);
                
    }
}


void AConceptCharacter::StartMovingAimTo(){
	if (!MyPlayerController){ return; }

	if(MyPlayerController->bIsUsingGamepad){
		SetLinearVectorToAim(AimVector2D);
	} else {
		float MouseX, MouseY;
		MyPlayerController->GetMousePosition(MouseX, MouseY);
		FVector2D CurrentMousePosition = FVector2D(MouseX, MouseY);

		if (CurrentMousePosition != LastMousePosition && MyPlayerController->IsLocalController()) {
			MyPlayerController->ProjectWorldLocationToScreen(GetActorLocation(), CharacterScreenBase2DLocation);
			MyPlayerController->CalculateMouse2DLocation();

			//CalculateWherePlayerIsLookingTo(MyPlayerController->Mouse2DLocation.X, CharacterScreenBase2DLocation.X);

			if (AbilityInUse) {
				FVector2D AimVector = MyPlayerController->CalculateAnimationAimPoint(AbilityInUse->GetPointToCalculateRotationToCursor(0), CurrentFloorOrientation);
				//AimRotation = FMath::Lerp(LastRotation, AimRotation, 0.8f);

				SetLinearVectorToAim(AimVector);
				LastRotation = AimVector;
			}
		}
		LastMousePosition = CurrentMousePosition;
	}

	
	AimingToDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::StartMovingAimTo);
	GetWorldTimerManager().SetTimer(AimingToHandler, AimingToDelegate, AimRefreshRate, false);
}

void AConceptCharacter::SetLinearVectorToAim_Implementation(FVector2D Aim){
	LinearVectorToAim = Aim;
}

void AConceptCharacter::SetLookingTo_Multicast_Implementation(ELookingToEnum::LookingTo Looking) {
	if (MyPlayerController) {
		LookingToSided = Looking;
	}
}

void AConceptCharacter::SetLookingTo_Implementation(ELookingToEnum::LookingTo Looking) {
	SetLookingTo_Multicast(Looking);
}

void AConceptCharacter::CalculateWherePlayerIsLookingTo(float MouseX, float CharacterX) {
	ELookingToEnum::LookingTo PlayerLookAt = (MouseX < CharacterX) ? ELookingToEnum::RE_LOOKINGTOLEFT : ELookingToEnum::RE_LOOKINGTORIGHT;
	if (PlayerLookAt != LookingToSided) {
		//SetLookingTo(PlayerLookAt);
	}

}

void AConceptCharacter::LerpControlRotation(FRotator Rotation) {
	if (MyPlayerController->GetControlRotation() != Rotation)
	{
		FTimerHandle Handle;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::LerpControlRotation, Rotation);
		GetWorldTimerManager().SetTimer(Handle, Delegate, 0.1f, false);
	}
}

void AConceptCharacter::UpdateCameraOffSet() {

	float x, y;
	float Distancia = 0;
    if(MyPlayerController){
        
        if (MyPlayerController->bIsUsingGamepad) {
            if(bGameAxisInactive || bAutoAim){
              DistanceOfAiming = ((GetActorUpVector() * LastMovementY * -1) + GetActorForwardVector() * LastMovementX * GetActorRightVector().X) * 900;
                DistanceOfAiming.X = -2300.f;
            } else {
                DistanceOfAiming = ((GetActorUpVector() * AxisY * -1) + GetActorForwardVector() * AxisX * GetActorRightVector().X) * 900;
                DistanceOfAiming.X = -2300.f;

            }
        }
        else if(MyPlayerController->GetMousePosition(x, y)) {
            MyPlayerController->CalculateWorldPositionOfMouse();
            DistanceOfAiming = GetActorLocation() - MyPlayerController->Mouse3DLocation;
        }
        
        FVector LocationToLookAt = GetActorLocation() + (DistanceOfAiming * CamaraRotationMultiplier);
		FVector FinalDirection = (LocationToLookAt - SideViewCameraComponent->GetComponentLocation());
		FinalDirection.Normalize();
		FRotator FinalRotation = FinalDirection.Rotation();
		FinalRotation.Roll = CurrentFloorOrientation.Roll;

		FQuat CurrentQuat(SideViewCameraComponent->GetComponentRotation());
		FQuat FinalQuat(FinalRotation);

		FQuat CalculatedQuat = FQuat::FastLerp(CurrentQuat, FinalQuat, CameraSpeedParam * GetWorld()->DeltaTimeSeconds);

		SideViewCameraComponent->SetWorldRotation(CalculatedQuat);


		FVector OffSetForced = FVector(350, 350, 350);
		OffSetForced *= GetActorUpVector() * GetWorld()->DeltaTimeSeconds;
		float JumpingMultipliyer = GetCharacterMovement()->IsFalling() ? -(GetMovementComponent()->Velocity.Size() / 8) : GetMovementComponent()->Velocity.Size() / 8;

		FVector OffSet = FMath::Lerp(CameraBoom->SocketOffset, FVector(JumpingMultipliyer + OffSetForced.X, -OffSetForced.Y, OffSetForced.Z), CameraSpeedParam * GetWorld()->DeltaTimeSeconds);

		CameraBoom->SocketOffset = OffSet;
	}
}

void AConceptCharacter::UpdateCameraOffSetWhenIsFree(){
	float x = 0;
	float y = 0;
	if (MyPlayerController->GetMousePosition(x, y)){ 

		//Quitar esto y ponerlo en el begin play o algo
		GEngine->GameViewport->Viewport->LockMouseToViewport(true);
		
		FVector OffsetIzq = FVector(0,0,0);
		FVector OffsetDer = FVector(0, 0, 0);
		FVector OffsetUp = FVector(0, 0, 0);
		FVector OffsetDown = FVector(0, 0, 0);
		
		int32 ViewportX, ViewportY;
		MyPlayerController->GetViewportSize(ViewportX, ViewportY);

		if(x >= ViewportX - 10) {
			OffsetIzq = FVector(0, 1, 0);
		}
		if(x <= 50) {
			OffsetDer = FVector(0,-1, 0);
		}
		if(y >= ViewportY - 10) {
			OffsetUp = FVector(0, 0, -1);
		}
		if(y <= 50) {
			OffsetDown = FVector(0, 0, 1);
		}


		FVector OffSet = (OffsetIzq + OffsetDer + OffsetUp + OffsetDown) * FreeCameraSpeedParam;
		OffSet = OffSet.RotateAngleAxis(-CurrentFloorOrientation.Roll, FVector(1, 0, 0));
		OffSet = FMath::Lerp(CameraBoom->SocketOffset, CameraBoom->SocketOffset + OffSet, CameraSpeedParam * GetWorld()->DeltaTimeSeconds);
		CameraBoom->SocketOffset = OffSet;

		return;



		MyPlayerController->CalculateWorldPositionOfMouse();

		float MouseX, MouseY;
		MouseX = (MyPlayerController->ViewportSize.X ) - x;
		MouseY = (MyPlayerController->ViewportSize.Y ) - y;

		FVector Distancia = FVector(0, - FMath::Pow((MyPlayerController->CenterOfViewport.X - MouseX) / -90, 5), - FMath::Pow((MyPlayerController->CenterOfViewport.Y - MouseY) / 60, 5));

		Distancia = Distancia.RotateAngleAxis(-CurrentFloorOrientation.Roll, FVector(1, 0, 0));
		
		float MultiplicadorIzquierda = FMath::Clamp(FMath::Abs(SideViewCameraComponent->GetComponentLocation().Y - 9000) / 1000, 0.0f, 1.0f);

		float MultiplicadorDerecha = FMath::Clamp(FMath::Abs(SideViewCameraComponent->GetComponentLocation().Y + 9000) / 1000, 0.0f, 1.0f);

		float MultiplicadorArriba = FMath::Clamp(FMath::Abs(SideViewCameraComponent->GetComponentLocation().Z - 9000) / 1000, 0.0f, 1.0f);

		float MultiplicadorAbajo = FMath::Clamp(FMath::Abs(SideViewCameraComponent->GetComponentLocation().Z + 9000) / 1000, 0.0f, 1.0f);

		Distancia = FVector(Distancia.X, Distancia.Y * MultiplicadorIzquierda * MultiplicadorDerecha, Distancia.Z * MultiplicadorArriba * MultiplicadorAbajo);
		Distancia *= GetWorld()->DeltaTimeSeconds * 10;

		CameraBoom->SocketOffset = OffSet;
	}
}

void AConceptCharacter::MoveCameraWithFreedom(bool ShouldMove) {
	if (ShouldMove){
		GetWorldTimerManager().ClearTimer(UpdatesBasedInMouseStatsHandle);
		UpdateCameraOffSetWhenIsFree();
	}
	else{
		GetWorldTimerManager().ClearTimer(UpdateCameraWhenIsFreeHandle);
		UpdatesBasedInMouseStats();
	}
}

void AConceptCharacter::MoveCameraWithFreedomPressed() {
	MoveCameraWithFreedom(true);
}

void AConceptCharacter::MoveCameraWithFreedomReleased() {
	MoveCameraWithFreedom(false);
}

void AConceptCharacter::CameraManager() {

	if(FreeCamera) {
		UpdateCameraOffSetWhenIsFree();
		return;
	}
	UpdatesBasedInMouseStats();

}

void AConceptCharacter::EnableShield_Implementation() {
		bIsShieldActivated = true;
		
		ShieldDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::ReduceShield, 5.0f);
		GetWorldTimerManager().SetTimer(ShieldHandle, ShieldDelegate, 0.1f, false);
	
}

void AConceptCharacter::DisableShield_Implementation() {
		bIsShieldActivated = false;
		GetWorldTimerManager().ClearTimer(ShieldHandle);
		GetWorldTimerManager().ClearTimer(ShieldRegenerationHandle);
		
		ShieldRegenerationDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::ShieldRegenerationSystem);
		GetWorldTimerManager().SetTimer(ShieldRegenerationHandle, ShieldRegenerationDelegate, 2.0f, false);
	
}

void AConceptCharacter::ToggleShield_Implementation() {

}


void AConceptCharacter::EnableShieldServer_Implementation() {
	if (!bIsShieldActivated) {
		EnableShield();
	}
}


void AConceptCharacter::DisableShieldServer_Implementation() {
	if (bIsShieldActivated) {
		DisableShield();
	}
}


void AConceptCharacter::ReduceShield(float Reduction) {
	if (Shield <= 0) { 
		DisableShieldServer();
		return;
	}

	else if (bIsShieldActivated) {
		SetShield(Shield - Reduction);

		bIsShieldActivated = true;
		ShieldDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::ReduceShield, Reduction);
		GetWorldTimerManager().SetTimer(ShieldHandle, ShieldDelegate, 0.1f, false);
	}
}

void AConceptCharacter::MoveRight(float Value) {
	if (!bUseGravity || !AbleToMove)
		return;
	RightAxisMovement = Value;
	MovementVector = FVector(0.0f, RightAxisMovement, UpAxisMovement);
	MovementVector *= -600;
	MovementPadVector = MovementVector + GetActorLocation();
	if (Value != 0.0f) {
		TimeSinceLastMovement = 0.0f;
		MoveRightValue = Value;
		MoveRightValue *= LookingToSided == ELookingToEnum::RE_LOOKINGTOLEFT ? -1 : 1;
		FVector forward = MapPawnOrientations[(uint8)LookingToSided].Vector();
		AddMovementInput(forward, MoveRightValue);

	}
	else{
		MoveRightValue = 0;
		TimeSinceLastMovement += GetWorld()->GetDeltaSeconds() / 10;
		if (TimeSinceLastMovement >= TimeToInactive){
			MyGameMode->InactivePlayer(MyPlayerController);
		}
	}
}

void AConceptCharacter::MoveUp(float Value) {
	UpAxisMovement = Value;
	FVector MovementVector = FVector(0.0f, RightAxisMovement, UpAxisMovement);
	MovementVector *= -600;
	MovementPadVector = MovementVector + GetActorLocation();
}

void AConceptCharacter::UpdatesBasedInMouseStats() {
	if (!ShouldTurn){ return; }
	UpdateCameraOffSet();
}

EOrientationToEnum::OrientedTo AConceptCharacter::MovingForward() {

	VelAux = GetTransform().InverseTransformVectorNoScale(GetVelocity());

	if (VelAux.X > 300){
	return EOrientationToEnum::RE_FORWARD;
	}

	if (VelAux.X < -300){ return  EOrientationToEnum::RE_BACKWARD; }

	return EOrientationToEnum::RE_ANY;

}

void AConceptCharacter::AbilityTestInUse(int32 index) {
	if (MyPlayerState && MyPlayerState->State == EStateEnum::RE_PLAYING) {
		if (MyPlayerState->CharacterAbilities[index]->bUniqueSimpleAbility) {
			if (bAbleToShoot)
			{
				bWantsFire = true;
				MyPlayerState->CharacterAbilities[index]->StartAbility();
				MyPlayerState->CharacterAbilities[index]->StopAbility();
				bWantsFire = false;
			}
		}
		else {
			StartChangeAbility(index);
		}
	}
}
template<int32 Index>
void AConceptCharacter::AbilityTestInUse() {
	AbilityTestInUse(Index);
}

void AConceptCharacter::StartChangeAbility_Implementation(int32 IndexOfWeaponToUse){
	if (IsChangingAbility || !AbilityInUse){ return; }

	if (AbilityInUse->StateOfAbility != EStateOfAbility::RE_INUSE) {
		AbilityInUse->StopInUseAbility();
		AbilityInUse->StopAbility();

		WeaponToChange.Add(IndexOfWeaponToUse);
		
		if (!AbilityInUse->ChangeAbilityMontage) {
			ChangeAbility();
			ChangeAbilityDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::EndChangeAbility);
			GetWorldTimerManager().SetTimer(ChangeAbilityHandle, ChangeAbilityDelegate, 0.1f, false);
			return;
		}

		AbilityInUse->Montage_Play_Multicast(AbilityInUse->ChangeAbilityMontage, 1, true);
		ChangeAbilityDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::EndChangeAbility);
		GetWorldTimerManager().SetTimer(ChangeAbilityHandle, ChangeAbilityDelegate, AbilityInUse->ChangeAbilityMontage->GetPlayLength(), false);
	}
	else if (!GetMesh()->AnimScriptInstance->Montage_IsPlaying(AbilityInUse->AbilityMontage) && !GetMesh()->AnimScriptInstance->Montage_IsPlaying(AbilityInUse->AbilityMontage)){
		AbilityInUse->IsFiring = false;
	}
}



void AConceptCharacter::ChangeAbility_Implementation(){
	if (!AbilityInUse) { return; }

	if (IndexOfAbilityInUse == WeaponToChange[WeaponToChange.Num() - 1]) {
		WeaponToChange.Top();
		return;
	}

	IndexOfAbilityInUse = WeaponToChange.Top();
	IncrementOfWheel = IndexOfAbilityInUse;
	if (!MyPlayerState) {return;}
	if (MyPlayerState->CharacterAbilities.Num() < 1 || !MyPlayerState->CharacterAbilities[IndexOfAbilityInUse]) {return;}

	FName SocketWeapon1 = AbilityInUse->CurrentSocketUsed;
	FName SocketWeapon2 = MyPlayerState->CharacterAbilities[IndexOfAbilityInUse]->CurrentSocketUsed;

	AbilityInUse->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketWeapon2);

	AbilityInUse->SetActorScale3D(FVector(1, 1, 1));
	AbilityInUse->CurrentSocketUsed = SocketWeapon2;

	AbilityInUse->StopInUseAbility();
	AbilityInUse->StopAbility();
	SetAbilityVisibility(false, AbilityInUse);
	AbilityInUse = MyPlayerState->CharacterAbilities[IndexOfAbilityInUse];
	//IncrementOfWheel = 0;

	AbilityInUse->CurrentSocketUsed = SocketWeapon1;

	AbilityInUse->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AbilityInUse->SocketToUse);
	AbilityInUse->SetActorScale3D(FVector(1, 1, 1));
	SetAbilityVisibility(true, AbilityInUse);
}


void AConceptCharacter::EndChangeAbility_Implementation() {
	if (!AbilityInUse) {return;}
		SetOrientationToMovement_Server(!AbilityInUse->OrientedToCursor);
		AbilityInUse->StartInUseAbility();
	
}

void AConceptCharacter::SetAbilityVisibility_Implementation(bool bIsVisible, AAbility* Ability) {
	Ability->SkeletalMeshComponent->SetVisibility(bIsVisible, true);
}

void AConceptCharacter::SetOrientationToMovement_Multicast_Implementation(bool OrientedToMovement)
{
	//GetCharacterMovement()->bOrientRotationToMovement = OrientedToMovement;
}

void AConceptCharacter::OtherHasReceivedDamage_Implementation(){

	if (bOtherHasReceivedDamage){
		bOtherHasReceivedDamage = false;
	}
	else{
		bOtherHasReceivedDamage = true;
		FTimerHandle Handle;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::OtherHasReceivedDamage);
		GetWorldTimerManager().SetTimer(Handle, Delegate, 0.3f, false);
	}
}

void AConceptCharacter::SetOrientationToMovement_Server_Implementation(bool OrientedToMovement)
{
	SetOrientationToMovement_Multicast(OrientedToMovement);
}


void AConceptCharacter::Reload()
{
	if (!AbilityInUse) { return; }
	AA_BaseWeapon* Weapon = Cast<AA_BaseWeapon>(AbilityInUse);
	if (Weapon) {
		Weapon->StartReload();
	}
}


void AConceptCharacter::FireMainAbility_Implementation() {
	bWantsFire = true;

	if(AbilityInUse && bAbleToShoot)
	{
		AbilityInUse->StartAbility();
		PlaySound_Client(TriggerCue, NULL, false,0.1f);
		OnSprint(false);
	}
}

void AConceptCharacter::FireSecondaryAbility_Implementation() {
	//if (AbilityInUse && bAbleToShoot)
	//{
	//	AbilityInUse->StartAbility();
	//}
}

void AConceptCharacter::PerformSpecialAbility_Implementation() {
	if (!MyPlayerState || !MyPlayerState->CharacterAbilities[3] || !MyPlayerState->CharacterAbilities[4]) {
		return;
	}

	if (MyPlayerState->CharacterAbilities[3]->bHaveSpecialBehaviour) {
		MyPlayerState->CharacterAbilities[3]->PerformSpecialBehavour();
	}
	else if (MyPlayerState->CharacterAbilities[4]->bHaveSpecialBehaviour) {
		MyPlayerState->CharacterAbilities[4]->PerformSpecialBehavour();
	}
}

void AConceptCharacter::StopMainAbility_Implementation() {
	bWantsFire = false;

	if (AbilityInUse)
	{
		AbilityInUse->StopAbility();
		OnSprint(SprintKeyPressed);
	}

	
}

void AConceptCharacter::StopSecondaryAbility_Implementation() {
	//if (AbilityInUse)
	//{
	//	AbilityInUse->StopAbility();
	//}
}

void AConceptCharacter::OnSprintPressed(){
	SprintKeyPressed = true;
	OnSprint(true);
}

void AConceptCharacter::OnSprintReleased(){
	SprintKeyPressed = false;
	OnSprint(false);
}



void AConceptCharacter::OnSprint(bool ShouldSprint){

	if (ShouldSprint && (AbleToMove && MoveRightValue != 0 && !GetMovementComponent()->IsFalling())){
		ServerSetSprintSpeed();
		IsSprinting = true;
		GetWorldTimerManager().ClearTimer(StaminaRegenerationSystemHandle);
		StaminaExpenseSystem();
		ShakeManager(OnSprintShakeClass, false, 1);
	}
	else{
		ServerSetNormalSpeed();
		IsSprinting = false;
		GetWorldTimerManager().ClearTimer(StaminaExpenseSystemHandle);
		StaminaRegenerationSystem();
		ShakeManager(OnSprintShakeClass, true, 1);
	}
}

void AConceptCharacter::StartStaminaRegeneration()
{
	StaminaRegenerationSystem();
}


ELookingToEnum::LookingTo AConceptCharacter::GetLookingTo()
{
	return LookingToSided;
}

void AConceptCharacter::ShakeManager(TSubclassOf<class UCameraShake> ShakeClass, bool bWantStop, float Scale){
	if (!MyPlayerController){ return; }
	if (bWantStop){
		MyPlayerController->ClientStopCameraShake(ShakeClass);
	}
	else{
		MyPlayerController->ClientPlayCameraShake(ShakeClass, Scale);
	}
}

void AConceptCharacter::LifeRegenerationSystem_Implementation(){
	if (Life != LifeMax){
		Life += (LifeReg / 10);
		if (Life > LifeMax){
			Life = LifeMax;
		}
	}

	IsRegenerating = true;
	LifeRegenerationDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::LifeRegenerationSystem);
	GetWorldTimerManager().SetTimer(LifeRegenerationHandle, LifeRegenerationDelegate, 0.1f, false);
}


void AConceptCharacter::ShieldRegenerationSystem_Implementation() {
	if (bIsShieldActivated) { return; }

	if (Shield != ShieldMax) {
		Shield += (ShieldReg / 10);
		if (Shield > ShieldMax) {
			Shield = ShieldMax;
			return;
		}
	}

	if (Shield < ShieldMax) {
		FTimerHandle Handle;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::ShieldRegenerationSystem);
		GetWorldTimerManager().SetTimer(Handle, Delegate, 0.1f, false);
	}
}

void AConceptCharacter::StaminaExpenseSystem_Implementation(){

	Stamina = FMath::Clamp(Stamina - StaminaExp, 0.0f, StaminaMax);

	if (Stamina == 0.0f){ OnSprint(false); }

	StaminaExpenseSystemDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::StaminaExpenseSystem);
	GetWorldTimerManager().SetTimer(StaminaExpenseSystemHandle, StaminaExpenseSystemDelegate, 0.1f, false);
}

void AConceptCharacter::StaminaRegenerationSystem_Implementation(){
	if (!bStaminaRegeneration) { return; }

	Stamina = FMath::Clamp(Stamina + StaminaReg, 0.0f, StaminaMax);
	//if (Stamina >= 5 && SprintKeyPressed){ OnSprint(true); }
	if (Stamina >= StaminaMax) {
		return;
	}

	StaminaRegenerationSystemDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::StaminaRegenerationSystem);
	GetWorldTimerManager().SetTimer(StaminaRegenerationSystemHandle, StaminaRegenerationSystemDelegate, 0.1f, false);
}

void AConceptCharacter::SetShield_Implementation(float ShieldParam)
{
	Shield = ShieldParam;
}



void AConceptCharacter::ServerSetLife_Implementation(float LifeParam)
{
	Life = LifeParam;
}



void AConceptCharacter::ServerSetLifeMax_Implementation(float LifeParam)
{
	LifeMax = LifeParam;
}



void AConceptCharacter::MulticastSetLifeReg_Implementation(float LifeRegParam) {
	LifeReg = LifeRegParam;
}



void AConceptCharacter::ServerSetLifeReg_Implementation(float SpeedParam) {
	MulticastSetLifeReg(SpeedParam);
}




void AConceptCharacter::MulticastSetJump_Implementation(float JumpParam) {
	SetCharacterJump(JumpParam);
}



void AConceptCharacter::ServerSetJump_Implementation(float JumpParam) {
	MulticastSetJump(JumpParam);
}



void AConceptCharacter::SetCanReceiveDamage_Implementation(bool CanReceive)
{
	CanReceiveDamage = CanReceive;
}



float AConceptCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {

	FAbilityData AbilityData;
	AProjectile* Projectile = Cast<AProjectile>(DamageCauser);
	AAbility* Ability = Cast<AAbility>(DamageCauser);

	UWorld* World = GetWorld();
	if (World->GetNetMode() != NM_ListenServer && World->GetNetMode() != NM_Standalone) { return 0.0f; }

	if (!CanReceiveDamage || Life <= 0) { return 0.0f; }

	if (Projectile) {
		if (Projectile->bAffectsShield && bIsShieldActivated) { return 0; }
		AbilityData = Projectile->AbilityData;
	}
	else if (Ability) {
		if (Ability->bAffectsShield && bIsShieldActivated) { return 0; }
		AbilityData = Ability->AbilityData;
	}

	float fDamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	TSubclassOf<UDamageType> DamageClaseWithoutCast = DamageEvent.DamageTypeClass;
	class UConceptDamageType* DamageClaseCasted = Cast<UConceptDamageType>(DamageClaseWithoutCast.GetDefaultObject());
	if (!DamageClaseCasted) {return 0.0f;}

    AController* DamagerController = Cast<AController>(EventInstigator);
    if(DamagerController){
        AConceptPlayerController* DamagerPlayerController = Cast<AConceptPlayerController>(EventInstigator);
        if (DamagerPlayerController) {
            if(DamagerPlayerController->MyPlayerState->Team == MyPlayerState->Team && DamagerPlayerController != MyPlayerController && !DamageClaseCasted->bAllowFriendlyFire ) {return 0.0f;}
            MyPlayerState->StatsAddDamageReceived(fDamageToApply);
            
            if (Damagers.Find(DamagerPlayerController) == -1) {
                Damagers.Add(DamagerPlayerController);
                TimesSinceLastDamage.Add(10);
            }
            else {TimesSinceLastDamage[Damagers.Find(DamagerPlayerController)];}
            
            if (DamagerPlayerController->MyPlayerState->Team == MyPlayerState->Team){
                DamagerPlayerController->MyPlayerState->StatsFriendlyFireDone();
            }
            
            AConceptCharacter* DamagerCharacter = Cast<AConceptCharacter>(DamagerPlayerController->GetPawn());
            if (DamagerCharacter) {
                //TODO: esto es la HUD moverlo al damagetype para que segun el daño, salte una HUD o otra
                DamagerCharacter->OtherHasReceivedDamage();
            }
            
            AConceptPlayerState* DamagerPlayerState = Cast<AConceptPlayerState>(DamagerPlayerController->PlayerState);
            if (DamagerPlayerState) {
                DamagerPlayerState->StatsAddDamageDone(fDamageToApply);
            }
        }
        
        APawn* DamagerCharacter = Cast<APawn>(DamagerController->GetPawn());
        if (DamagerCharacter){
            MyPlayerController->OnDamageReceived(DamagerCharacter);
        }
        
        if (HasAuthority() && GetController()) {
            
			DamageClaseCasted->HandleDamage(fDamageToApply, EventInstigator, GetController(), MyDamageBehavior, MyGameMode, AbilityData);
        }
    }
    
    return fDamageToApply;
}


void AConceptCharacter::OnLifeDamageReceive_Implementation(float Damage, TSubclassOf<UDamageType> DamageType) {

}

void AConceptCharacter::ApplyDamageMomentum(float DamageTaken, const FDamageEvent& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	const UDamageType* DmgTypeCDO = DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>();
	const float ImpulseScale = DmgTypeCDO->DamageImpulse;

	UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
	if (ImpulseScale > 3.0f && CharacterMovement != NULL)
	{
		FHitResult HitInfo;
		FVector ImpulseDir;
		DamageEvent.GetBestHitInfo(this, PawnInstigator, HitInfo, ImpulseDir);

		FVector Impulse = ImpulseDir * ImpulseScale;
		const bool bMassIndependentImpulse = !DmgTypeCDO->bScaleMomentumByMass;

		// Limit Z momentum added if already going up faster than jump (to avoid blowing character way up into the sky).
		{
			FVector MassScaledImpulse = Impulse;
			if (!bMassIndependentImpulse && CharacterMovement->Mass > SMALL_NUMBER)
			{
				MassScaledImpulse = MassScaledImpulse / CharacterMovement->Mass;
			}

			const FVector AxisZ = GetActorQuat().GetAxisZ();
			if ((CharacterMovement->Velocity | AxisZ) > GetDefault<UCharacterMovementComponent>(CharacterMovement->GetClass())->JumpZVelocity && (MassScaledImpulse | AxisZ) > 0.0f)
			{
				Impulse = FVector::VectorPlaneProject(Impulse, AxisZ) + AxisZ * ((Impulse | AxisZ) * 0.5f);
			}
		}

		CharacterMovement->AddImpulse(Impulse, bMassIndependentImpulse);
	}
}

FVector AConceptCharacter::GetPawnViewLocation() const
{
	return GetActorLocation() + GetActorQuat().GetAxisZ() * BaseEyeHeight;
}

void AConceptCharacter::PostNetReceiveLocationAndRotation()
{
	
	// Always consider Location as changed if we were spawned this tick as in that case our replicated Location was set as part of spawning, before PreNetReceive().
	if (ReplicatedMovement.Location == GetActorLocation() && ReplicatedMovement.Rotation == GetActorRotation() && CreationTime != GetWorld()->TimeSeconds)
	{
		return;
	}

	if (Role == ROLE_SimulatedProxy)
	{
		const FVector OldLocation = GetActorLocation();
		const FQuat OldRotation = GetActorQuat();
		const FQuat NewRotation = ReplicatedMovement.Rotation.Quaternion();

		// Correction to make sure pawn doesn't penetrate floor after replication rounding.
		ReplicatedMovement.Location += NewRotation.GetAxisZ() * 0.01f;

		SetActorLocationAndRotation(ReplicatedMovement.Location, ReplicatedMovement.Rotation, /*bSweep=*/ false);

		INetworkPredictionInterface* PredictionInterface = Cast<INetworkPredictionInterface>(GetMovementComponent());
		if (PredictionInterface)
		{
			PredictionInterface->SmoothCorrection(OldLocation, OldRotation, ReplicatedMovement.Location, NewRotation);
		}
	}
}

void AConceptCharacter::LaunchCharacterRotated(FVector LaunchVelocity, bool bHorizontalOverride, bool bVerticalOverride)
{
	UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
	if (CharacterMovement)
	{
		if (!bHorizontalOverride && !bVerticalOverride)
		{
			CharacterMovement->Launch(GetVelocity() + LaunchVelocity);
		}
		else if (bHorizontalOverride && bVerticalOverride)
		{
			CharacterMovement->Launch(LaunchVelocity);
		}
		else
		{
			FVector FinalVel;
			const FVector Velocity = GetVelocity();
			const FVector AxisZ = GetActorQuat().GetAxisZ();

			if (bHorizontalOverride)
			{
				FinalVel = FVector::VectorPlaneProject(LaunchVelocity, AxisZ) + AxisZ * (Velocity | AxisZ);
			}
			else // if (bVerticalOverride)
			{
				FinalVel = FVector::VectorPlaneProject(Velocity, AxisZ) + AxisZ * (LaunchVelocity | AxisZ);
			}

			CharacterMovement->Launch(FinalVel);
		}

		OnLaunched(LaunchVelocity, bHorizontalOverride, bVerticalOverride);
	}
}

FORCEINLINE class UCustomCharacterMovementComponent* AConceptCharacter::GetCustomCharacterMovement() const
{
	return Cast<UCustomCharacterMovementComponent>(GetMovementComponent());
}

void AConceptCharacter::SubstractDamage() {
}


void AConceptCharacter::MakeRadialForce(float Radius, float Force, float TimeToStop, bool bShouldStop) {
	if (bShouldStop) {
		RadialForceComponent->Deactivate();
	}
	else {
		RadialForceComponent->ForceStrength = Force;
		RadialForceComponent->Radius = Radius;
		RadialForceComponent->Activate();
		
		FTimerHandle Handle;
		FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::MakeRadialForce, Radius, Force, TimeToStop, true);
		GetWorldTimerManager().SetTimer(Handle, delegate, TimeToStop, false);
	}	
}




void AConceptCharacter::Death_Implementation() {
	if (MyPlayerState) {
		for (int32 i = 0; i < Damagers.Num() - 1; i++) {
			Damagers[i]->MyPlayerState->ScoreAssist();
		}
	}

	GetWorldTimerManager().ClearTimer(ShieldRegenerationHandle);
	GetWorldTimerManager().ClearTimer(LifeRegenerationHandle);
	
	
	PlaySound(DeathCue,NULL, false);
	StopSound_Server(BreathSoundComponent);
	//ShakeManager(OnSprintShakeClass, true, 1);
	//ShakeManager(OnDamageShakeClass, true, 1);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//SetCapsuleCollisionResponseOnServer(GetCapsuleComponent(), ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ShouldTurn = false;
	IsVisible = false;
	bCanBeDamaged = false;
	ResetStatus();

	EnablePhysics(0.0f);

	FTimerHandle Handle;
	FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::DissolveMaterialEffect);
	GetWorldTimerManager().SetTimer(Handle, delegate, 5.0f, false);

}

void AConceptCharacter::BeginDestroy() {
	//Death();
	Super::BeginDestroy();
}

void AConceptCharacter::EnablePhysics_Implementation(float Time) {
	//GetMovementComponent()->SetComponentTickEnabled(false);
	//GetMesh()->SetAllBodiesBelowSimulatePhysics("Pelvis", true);
}

void AConceptCharacter::OnCharacterHitted_Implementation(float Time) {

}

void AConceptCharacter::PlayerStartHover() {
	AConceptCharacter* CurrentCharacter = Cast<AConceptCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (CurrentCharacter){
		CurrentCharacter->SetTarget((AActor*)this);
	}
	GetMesh()->SetRenderCustomDepth(true);
}

void AConceptCharacter::PlayerExitHover() {
	AConceptCharacter* CurrentCharacter = Cast<AConceptCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (CurrentCharacter){
		CurrentCharacter->SetTarget(NULL);
		CurrentCharacter->SelectedTarget = NULL;
	}
}

void AConceptCharacter::MakeRoll_Implementation(){
	MyPlayerState->StatsAddRolls();
}
void AConceptCharacter::ToggleAutoAim_Implementation() {

}


void AConceptCharacter::CustomTurnOff_Implementation() {
	//SetActorEnableCollision(false);

	if (GetCharacterMovement()) {
		//GetCharacterMovement()->StopMovementImmediately();
		//GetCharacterMovement()->DisableMovement();
		//GetCharacterMovement()->DisableMovement();
		AbleToMove = false;
	}
}

void AConceptCharacter::ReactiveCharacter_Implementation() {
	SetActorEnableCollision(true);

	////------Temporary fixes------
	//bCanBeDamaged = true;
	//ChangeAbility();
	////---------------------------

	if (GetMovementComponent()) {
		GetMovementComponent()->SetComponentTickEnabled(true);
	}

	EnableMovement();
	InitState();
	InitMouse();

	if (GetNetMode() != NM_DedicatedServer && GetMesh() != NULL) {
		GetMesh()->bPauseAnims = false;
		GetMesh()->bBlendPhysics = true;
		GetMesh()->KinematicBonesUpdateType = EKinematicBonesUpdateToPhysics::SkipSimulatingBones;
	}
}

float AConceptCharacter::SetCharacterSpeed(float SpeedParam){
	UCharacterMovementComponent *MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = SpeedParam * VelocityMultiplierOnUse;
	return Speed;
}

void AConceptCharacter::MulticastSetSpeed_Implementation(float SpeedParam){
	SetCharacterSpeed(SpeedParam);
}

void AConceptCharacter::ServerSetSpeed_Implementation(float SpeedParam){
	MulticastSetSpeed(SpeedParam);
}

void AConceptCharacter::MulticastSetSprintSpeed_Implementation() { SetCharacterSpeed(SprintSpeed);}

void AConceptCharacter::ServerSetSprintSpeed_Implementation() { MulticastSetSprintSpeed();}

void AConceptCharacter::MulticastSetNormalSpeed_Implementation() { SetCharacterSpeed(Speed);}

void AConceptCharacter::ServerSetNormalSpeed_Implementation() { MulticastSetNormalSpeed();}




float AConceptCharacter::SetCharacterJump(float JumpParam) {
	UCharacterMovementComponent *MovementComponent = GetCharacterMovement();
	JumpPower = JumpParam;
	MovementComponent->JumpZVelocity = JumpPower;

	return JumpPower;
}

void AConceptCharacter::Server_MakeInvisible_Implementation(UMaterialInterface* NewMat, bool Visible = true) { 
	//Death();
	MakeInvisible(NewMat, Visible); 
}

void AConceptCharacter::MakeInvisible_Implementation(UMaterialInterface* NewMat, bool isVisible){
	//RootComponent->SetVisibility(isVisible, true);
	GetMesh()->SetVisibility(isVisible, true);
	if(!bIsShieldActivated && isVisible){
		DisableShield();
		TArray<UActorComponent*> Scenes;
		UStaticMeshComponent* PlayerMeshComponent;
		PlayerMeshComponent = Cast<UStaticMeshComponent>(UStaticFunctionLibrary::GetSpecificComponent(Scenes, "MeshOfShield", this));
		if(PlayerMeshComponent){
			PlayerMeshComponent->SetVisibility(false, false);
		}
	}
	this->IsVisible = isVisible;

	if (NewMat == NULL) {
		for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++){
			GetMesh()->SetMaterial(i, DefaultsMaterial[i]);
		}
		return;
	}
	AConceptPlayerController* CurrentController = Cast<AConceptPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentController && CurrentController->MyPlayerState->Team == this->MyPlayerState->Team) {
		//TODO: buscar weaponmeshes y hacer change material
		for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++) {
			GetMesh()->SetMaterial(i, NewMat);
			GetMesh()->SetVisibility(true);
		}
	}
	
	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::MakeInvisible, (UMaterialInterface*) NULL, true);
	GetWorldTimerManager().SetTimer(Handle, Delegate, 5.0f, false);
}

void AConceptCharacter::SetTarget_Implementation(AActor* Target){SelectedTarget = Target;}




void AConceptCharacter::GravityRotatorCalculator(float DeltaSeconds){

	TArray<FRotator> RightOrientations;
	TArray<FRotator> LeftOrientations;
	TArray<FRotator> FloorOrientations;
	TArray<FVector> Gravitys;
	TArray<AActor*> TriangleArea;
	TArray<AHiveGravityZone*> TriangleAreaCasted;
	
	GetCapsuleComponent()->GetOverlappingActors(TriangleArea, TSubclassOf<AHiveGravityZone>());
	GetCustomCharacterMovement()->UpdateComponentRotation();

	for (int32 i = 0; i != TriangleArea.Num(); ++i) {
		AHiveGravityZone* GravityArea = Cast<AHiveGravityZone>(TriangleArea[i]);
		if (GravityArea) {
			TriangleAreaCasted.Add(GravityArea);
		}
	}

	//if (TriangleAreaCasted.Num() == 1 && !isLerpingGravity) {
	//	return;
	//}

	isLerpingGravity = true;

	for (int32 i = 0; i != TriangleAreaCasted.Num(); ++i) {
		AHiveGravityZone* GravityArea = TriangleAreaCasted[i];
		if (GravityArea) {
			Gravitys.Add(GravityArea->GetActorUpVector() * (-1));
			LeftOrientations.Add(GravityArea->Left);
			RightOrientations.Add(GravityArea->Right);
			FloorOrientations.Add(GravityArea->GetActorRotation());
		}
	}

	for (int32 i = 0; i != Gravitys.Num(); ++i) {
		if (i == 0) {
			Gravity = Gravitys[i];
			LeftOrientation = LeftOrientations[i];
			RightOrientation = RightOrientations[i];
			FloorOrientation = FRotator(FloorOrientations[i].Pitch, FloorOrientations[i].Yaw, FloorOrientations[i].Roll*-1);
		}
		else {
			Gravity = FMath::Lerp(Gravity, Gravitys[i], 0.3f);
			LeftOrientation = RotationLerp(LeftOrientation, LeftOrientations[i], 0.5f);
			RightOrientation = RotationLerp(RightOrientation, RightOrientations[i], 0.5f);
			FloorOrientation = RotationLerp(FloorOrientation, (FRotator(FloorOrientations[i].Pitch, FloorOrientations[i].Yaw, FloorOrientations[i].Roll*-1)), 0.5f);
		}
	}

	RotationSpeed = 15.5f;
	Gravity = FMath::Lerp(GetCustomCharacterMovement()->GetGravityDirection(), Gravity, RotationSpeed * DeltaSeconds);
	LeftOrientation = RotationLerp(MapPawnOrientations[1], LeftOrientation, RotationSpeed * DeltaSeconds);
	RightOrientation = RotationLerp(MapPawnOrientations[0], RightOrientation, RotationSpeed * DeltaSeconds);
	FloorOrientation = RotationLerp(CurrentFloorOrientation, FloorOrientation, RotationSpeed * DeltaSeconds);


	if (MapPawnOrientations[0] == RightOrientation && MapPawnOrientations[1] == LeftOrientation) {
		isLerpingGravity = false;
		return;
	}

	
	GetCustomCharacterMovement()->SetGravityDirection(Gravity);
	

	MapPawnOrientations[0] = RightOrientation;
	MapPawnOrientations[1] = LeftOrientation;
	CurrentFloorOrientation = FloorOrientation;

	
}

void AConceptCharacter::ZeroGravityRotatorCalculator(float DeltaSeconds)
{
	TArray<AActor*> ZeroGravityZone;

	GetCapsuleComponent()->GetOverlappingActors(ZeroGravityZone, TSubclassOf<AP_ZeroGravityZone>());
	
	bool bIsInsideOfZone = false;
	for (auto& Zone : ZeroGravityZone) {
		if (Zone->IsA(AP_ZeroGravityZone::StaticClass())) {
			bIsInsideOfZone = true;
		}
	}
	if (!bIsInsideOfZone) {
		ReplicateGravity(false, 0.0f);
		return;
	}

	GetCharacterMovement()->Velocity = ZeroGravitySpeed;
	FRotator ActorRotation = GetActorRotation();
	ZeroGravityRotation.Pitch += 45 * DeltaSeconds * DirectionZeroG;
	PreviousZeroGRotation.Pitch += 45 * DeltaSeconds * DirectionZeroG;

	SetActorRotation(ZeroGravityRotation.Quaternion());
	//SetActorRotation(PreviousZeroGRotation.Quaternion());
	//SetActorRotation(ActorRotation.Quaternion());
}

FRotator AConceptCharacter::RotationLerp(FRotator InitRotation, FRotator FinalRotation, float alpha) {

	FQuat InitQuat(InitRotation);
	FQuat FinalQuat(FinalRotation);
	FQuat LerpRotation = FQuat::FastLerp(InitQuat, FinalQuat, alpha);
	LerpRotation.Normalize();
	return LerpRotation.Rotator();

}

void AConceptCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AConceptCharacter, Life);
	DOREPLIFETIME(AConceptCharacter, LifeMax);
	DOREPLIFETIME(AConceptCharacter, Shield);
	DOREPLIFETIME(AConceptCharacter, ShieldMax);
	DOREPLIFETIME(AConceptCharacter, AbilityInUse);
	DOREPLIFETIME(AConceptCharacter, MyPlayerState);
	DOREPLIFETIME(AConceptCharacter, MyPlayerController);
	DOREPLIFETIME(AConceptCharacter, CharacterOpacity);
	DOREPLIFETIME(AConceptCharacter, SelectedTarget);
	DOREPLIFETIME(AConceptCharacter, FiringType);
	DOREPLIFETIME(AConceptCharacter, WeaponToChange);
	DOREPLIFETIME(AConceptCharacter, LinearVectorToAim);
	DOREPLIFETIME(AConceptCharacter, LookingToSided);
	DOREPLIFETIME(AConceptCharacter, IsSprinting);	
	DOREPLIFETIME(AConceptCharacter, InversePlayer);
	DOREPLIFETIME(AConceptCharacter, VelocityMultiplierOnUse);
	DOREPLIFETIME(AConceptCharacter, IndexOfAbilityInUse);
	DOREPLIFETIME(AConceptCharacter, IncrementOfWheel);
	
}

void AConceptCharacter::DissolveMaterialEffect_Implementation() {
	//IMPLEMENTED IN BLUEPRINT
}

void AConceptCharacter::GameFinished_Implementation() {
	//IMPLEMENTED IN BLUEPRINT
}


template <typename T>
T* AConceptCharacter::GetSpecificComponent(TArray<T*> &Components, FString ComponentName) {
	this->GetComponents(Components);
	for (auto Component : Components){
		if (Component->GetName() == ComponentName){
			return Component;
		}
	}
	return NULL;
}

void AConceptCharacter::CanShoot_Implementation(bool bCanShoot)
{
	bAbleToShoot = bCanShoot;
	CanShoot_Clients(bCanShoot);
}


void AConceptCharacter::CanShoot_Clients_Implementation(bool bCanShoot)
{
	bAbleToShoot = bCanShoot;
}

void AConceptCharacter::CantShootDamage()
{
	CanShoot(false);
	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::RestoreAbleToSoot);
	GetWorldTimerManager().SetTimer(Handle, Delegate, 0.5f, false);
}

void AConceptCharacter::RestoreAbleToSoot()
{
	CanShoot(true);
}


void AConceptCharacter::ReplicateGravity_Implementation(bool bActivateZeroGravity, float Direction)
{
	DirectionZeroG = Direction;

	ReplicateGravity_Clients(bActivateZeroGravity, Direction);

	if (bActivateZeroGravity)
	{
		ActivateZeroGravity();
	}
	else
		DeactivateZeroGravity();

}



void AConceptCharacter::ReplicateGravity_Clients_Implementation(bool bActivateZeroGravity, float Direction)
{
	DirectionZeroG = Direction;

	if (bActivateZeroGravity)
	{
		ActivateZeroGravity();
	}
	else
		DeactivateZeroGravity();
}

void AConceptCharacter::OnGetAchievement()
{
	UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	if (GameInstance) {
		GameInstance->SetAchievement(MyPlayerController, "FUCK_YOU_0", 70);
	}

}


/*--------STATUS FUNCTIONS--------*/
void AConceptCharacter::OnCCStatusUpdated_Implementation() {
}
void AConceptCharacter::CCStatusUpdater(float SlowAmount) {
	bAbleToShoot = true;
	AbleToMove = true;
	bAbleToDash = true;
	OnCCStatusUpdated();
    GetCustomCharacterMovement()->MaxWalkSpeed = Speed;
	if (CharacterCCStatus.Contains(ECharacterCCStatusEnum::RE_STUNNED)) {
		bAbleToShoot = false;
		AbleToMove = false;
		bAbleToDash = false;
	} 
	if (CharacterCCStatus.Contains(ECharacterCCStatusEnum::RE_DISRUPTED)) {
		bAbleToShoot = false;
		bAbleToDash = false;
		
	}
	if (CharacterCCStatus.Contains(ECharacterCCStatusEnum::RE_SNARED)) {
		AbleToMove = false;
		bAbleToDash = false;
	}
	if (CharacterCCStatus.Contains(ECharacterCCStatusEnum::RE_SLOWED)) {
        GetCustomCharacterMovement()->MaxWalkSpeed = Speed/SlowAmount;
	}

}

void AConceptCharacter::StartDisruptStatusServer_Implementation(float DisruptTime) {
	StartDisruptStatus(DisruptTime);
}

void AConceptCharacter::StartDisruptStatus_Implementation(float DisruptTime) {
	CharacterCCStatus.AddUnique(ECharacterCCStatusEnum::RE_DISRUPTED);
	CCStatusUpdater();

	GetWorldTimerManager().ClearTimer(DisruptHandle);
	DisruptDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::EndDisruptStatus);
	GetWorldTimerManager().SetTimer(DisruptHandle, DisruptDelegate, DisruptTime, false);
}
void AConceptCharacter::EndDisruptStatus() {
	GetWorldTimerManager().ClearTimer(DisruptHandle);
	CharacterCCStatus.Remove(ECharacterCCStatusEnum::RE_DISRUPTED);
	CCStatusUpdater();
}

void AConceptCharacter::EndConstrictionStatus() {
	GetWorldTimerManager().ClearTimer(ConstrictionHandle);
	CharacterCCStatus.Remove(ECharacterCCStatusEnum::RE_STUNNED);
	CCStatusUpdater();
}


void AConceptCharacter::StartStunStatusServer_Implementation(float StunTime) {
	StartStunStatus(StunTime);
}
bool AConceptCharacter::StartStunStatusServer_Validate(float StunTime) {
	return true;
}
void AConceptCharacter::StartStunStatus_Implementation(float StunTime) {
	CharacterCCStatus.AddUnique(ECharacterCCStatusEnum::RE_STUNNED);
	CCStatusUpdater();

	GetWorldTimerManager().ClearTimer(StunHandle);
	StunDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::EndStunStatus);
	GetWorldTimerManager().SetTimer(StunHandle, StunDelegate, StunTime, false);
}
void AConceptCharacter::EndStunStatus() {
	GetWorldTimerManager().ClearTimer(StunHandle);
	CharacterCCStatus.Remove(ECharacterCCStatusEnum::RE_STUNNED);
	CCStatusUpdater();
}

void AConceptCharacter::StartSlowStatusServer_Implementation(float SlowTime, float SlowAmount) {
    StartSlowStatus(SlowTime, SlowAmount);
}
bool AConceptCharacter::StartSlowStatusServer_Validate(float SlowTime, float SlowAmount) {
    return true;
}
void AConceptCharacter::StartSlowStatus_Implementation(float SlowTime, float SlowAmount) {
    CharacterCCStatus.AddUnique(ECharacterCCStatusEnum::RE_SLOWED);
    CCStatusUpdater(SlowAmount);
    
    GetWorldTimerManager().ClearTimer(SlowHandle);
    SlowDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::EndSlowStatus);
    GetWorldTimerManager().SetTimer(SlowHandle, SlowDelegate, SlowTime, false);
}
void AConceptCharacter::EndSlowStatus() {
    GetWorldTimerManager().ClearTimer(SlowHandle);
    CharacterCCStatus.Remove(ECharacterCCStatusEnum::RE_SLOWED);
    CCStatusUpdater();
}

void AConceptCharacter::StartSnareStatusServer_Implementation(float SnareTime) {
	StartSnareStatus(SnareTime);
}
bool AConceptCharacter::StartSnareStatusServer_Validate(float SnareTime) {
	return true;
}
void AConceptCharacter::StartSnareStatus_Implementation(float SnareTime) {
	CharacterCCStatus.AddUnique(ECharacterCCStatusEnum::RE_SNARED);
	CCStatusUpdater();

	GetWorldTimerManager().ClearTimer(SnareHandle);
	DisruptDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::EnableMovement);
	GetWorldTimerManager().SetTimer(SnareHandle, DisruptDelegate, SnareTime, false);
}


void AConceptCharacter::ToggleAbilities(bool ActiveStatus) {
	bAbleToShoot = ActiveStatus;
	bAbleToDash = ActiveStatus;
}

void AConceptCharacter::DisableMovement() {
	if(CharacterCCStatus.Contains(ECharacterCCStatusEnum::RE_STUNNED)) 
	if (CharacterCCStatus.Contains(ECharacterCCStatusEnum::RE_STUNNED) || CharacterCCStatus.Contains(ECharacterCCStatusEnum::RE_DISRUPTED)) {
		bAbleToShoot = false;
	}
	AbleToMove = false;
	bAbleToDash = false;
}

void AConceptCharacter::EnableMovement() {
	if (GetCharacterMovement()->GetCharacterOwner()) {
		GetCharacterMovement()->SetDefaultMovementMode();
	}
	else {
		GetCharacterMovement()->MovementMode = MOVE_Walking;
		GetCharacterMovement()->CustomMovementMode = 0;
	}
	AbleToMove = true;
	bAbleToShoot = true;
	bAbleToDash = true;
	bUseGravity = true;
}







//Borja
void AConceptCharacter::UpdateStatus(float DeltaTime)
{
	UpdatePoison(DeltaTime);
	UpdateBurn(DeltaTime);
	UpdateConstriction(DeltaTime);
}

void AConceptCharacter::ResetStatus()
{
	//POISON
	PoisonEnemy = nullptr;
	PoisonStatusTimeLeft = 0.0f;
	PoisonStatusTimer = 5.0f;
	PoisonDamageTimeLeft = 0.0f;
	PoisonDamageTimer = 0.2f;
	PoisonDamage = 0.0f;
	bPoisoned = false;

	//BURN
	BurnEnemy = nullptr;
	BurnStatusTimeLeft = 0.0f;
	BurnStatusTimer = 5.0f;
	BurnDamageTimeLeft = 0.0f;
	BurnDamageTimer = 0.2f;
	BurnDamage = 0.0f;
	bBurning = false;


	//CONSTICTION
	ConstrictionEnemy=nullptr;
	ConstrictionStatusTimeLeft=0.0f;
	ConstrictionStatusTimer=2.0f;
	ConstrictionDamageTimeLeft=0.0f;
	ConstrictionDamageTimer=0.2f;
	ConstrictionDamage=0.0f;
	bConstrictioning = false;

}

//POISON STATUS
void AConceptCharacter::SetPoisonStatus(AController* Enemy, float Damage, float poisonStatusTimeLeft, float poisonDamageTimeLeft)
{
	PoisonEnemy = Enemy;
	PoisonDamage = Damage;
	PoisonStatusTimeLeft = poisonStatusTimeLeft;
	PoisonDamageTimeLeft = poisonDamageTimeLeft;
	bPoisoned = true;

}
void AConceptCharacter::UpdatePoison(float DeltaTime)
{
	if (PoisonStatusTimeLeft <= 0.0f)
		return;


	if (PoisonDamageTimeLeft <= 0.0f || !PoisonEnemy)
	{
		if (FMath::Abs(PoisonDamageTimeLeft) > PoisonDamageTimer * 2)
		{
			for (int32 i = 0; i < FMath::Abs(PoisonDamageTimeLeft) / PoisonDamageTimer; i++)
				UGameplayStatics::ApplyDamage(this, PoisonDamage, PoisonEnemy, (AActor*)PoisonEnemy, UD_PoisonDamage::StaticClass());//MyDamageBehavior->ApplyLifeDamage(PoisonDamage);
		}
		else
		{
			UGameplayStatics::ApplyDamage(this, PoisonDamage, PoisonEnemy, (AActor*)PoisonEnemy, UD_PoisonDamage::StaticClass());
		}
		PoisonDamageTimeLeft = PoisonDamageTimer;
	}

	PoisonStatusTimeLeft -= DeltaTime;
	PoisonDamageTimeLeft -= DeltaTime;

	if (PoisonStatusTimeLeft <= 0.0f)
	{
		bPoisoned = false;

		if(!IsRegenerating)
			LifeRegenerationSystem();
	}
}

//BURN STATUS
void AConceptCharacter::SetBurnStatus(AController* Enemy, float Damage)
{
	BurnEnemy = Enemy;
	BurnDamage = Damage;
	BurnStatusTimeLeft = BurnStatusTimer;
	BurnDamageTimeLeft = BurnDamageTimer;
	bBurning = true;

}

void AConceptCharacter::UpdateBurn(float DeltaTime)
{
	if (BurnStatusTimeLeft <= 0.0f || !BurnEnemy)
		return;

	float NewDamage = BurnStatusTimeLeft / BurnStatusTimer * (BurnDamage * 0.75f);

	if (BurnDamageTimeLeft <= 0.0f)
	{
		if (FMath::Abs(BurnDamageTimeLeft) > BurnDamageTimer * 2)
		{
			for (int32 i = 0; i < FMath::Abs(BurnDamageTimeLeft) / BurnDamageTimer; i++)
				UGameplayStatics::ApplyDamage(this, NewDamage, BurnEnemy, (AActor*)BurnEnemy, UD_BurnDamage::StaticClass());//MyDamageBehavior->ApplyLifeDamage(PoisonDamage);
		}
		else
		{
			UGameplayStatics::ApplyDamage(this, NewDamage, BurnEnemy, (AActor*)BurnEnemy, UD_BurnDamage::StaticClass());
		}
		BurnDamageTimeLeft = BurnDamageTimer;
	}

	BurnStatusTimeLeft -= DeltaTime;
	BurnDamageTimeLeft -= DeltaTime;

	if (BurnStatusTimeLeft <= 0.0f)
	{
		bBurning = false;

		if (!IsRegenerating)
			LifeRegenerationSystem();
	}
}

void AConceptCharacter::SetConstrictionStatus(AController* Enemy, float Damage, float constrictionStatusTimeLeft, float constrictionDamageTimeLeft)
{
	/*ConstrictionEnemy = Enemy;
	ConstrictionDamage = Damage;
	ConstrictionStatusTimeLeft = constrictionStatusTimeLeft;
	ConstrictionDamageTimeLeft = constrictionDamageTimeLeft;
	bConstrictioning = true;*/

	CharacterCCStatus.Add(ECharacterCCStatusEnum::RE_STUNNED);
	CCStatusUpdater();

	//GetWorldTimerManager().ClearTimer(ConstrictionHandle);
	ConstrictionDelegate = FTimerDelegate::CreateUObject(this, &AConceptCharacter::EndConstrictionStatus);
	GetWorldTimerManager().SetTimer(ConstrictionHandle, ConstrictionDelegate, constrictionStatusTimeLeft, false);

}


void AConceptCharacter::UpdateConstriction(float DeltaTime)
{
	if (ConstrictionStatusTimeLeft <= 0.0f || !ConstrictionEnemy)
		return;

	float NewDamage = ConstrictionStatusTimeLeft / ConstrictionStatusTimer * (ConstrictionDamage * 0.75f);

	if (ConstrictionDamageTimeLeft <= 0.0f)
	{
		if (FMath::Abs(ConstrictionDamageTimeLeft) > ConstrictionDamageTimer * 2)
		{
			for (int32 i = 0; i < FMath::Abs(ConstrictionDamageTimeLeft) / ConstrictionDamageTimer; i++)
				UGameplayStatics::ApplyDamage(this, NewDamage, ConstrictionEnemy, (AActor*)ConstrictionEnemy, UD_ConstrictorDamage::StaticClass());//MyDamageBehavior->ApplyLifeDamage(PoisonDamage);
		}
		else
		{
			UGameplayStatics::ApplyDamage(this, NewDamage, ConstrictionEnemy, (AActor*)ConstrictionEnemy, UD_ConstrictorDamage::StaticClass());
		}
		ConstrictionDamageTimeLeft = ConstrictionDamageTimer;
	}

	ConstrictionStatusTimeLeft -= DeltaTime;
	ConstrictionDamageTimeLeft -= DeltaTime;

	if (BurnStatusTimeLeft <= 0.0f)
	{
		bBurning = false;

		if (!IsRegenerating)
			LifeRegenerationSystem();
	}
}



void AConceptCharacter::UsePowerUp() {}
bool AConceptCharacter::SetTarget_Validate(AActor* Target) { return true; }
bool AConceptCharacter::LifeRegenerationSystem_Validate() { return true; }
bool AConceptCharacter::ShieldRegenerationSystem_Validate() { return true; }
bool AConceptCharacter::SetLinearVectorToAim_Validate(FVector2D VectorToAim) { return true; }
bool AConceptCharacter::SetLookingTo_Validate(ELookingToEnum::LookingTo Looking) { return true; }
bool AConceptCharacter::SetLookingTo_Multicast_Validate(ELookingToEnum::LookingTo Looking) { return true; }
bool AConceptCharacter::StartChangeAbility_Validate(int32 IndexOfWeaponToUse) { return true; }
bool AConceptCharacter::ChangeAbility_Validate() { return true; }
bool AConceptCharacter::EndChangeAbility_Validate() { return true; }
bool AConceptCharacter::SetCapsuleCollisionResponseOnServer_Validate(UPrimitiveComponent* Primitive, ECollisionChannel Channel, ECollisionResponse NewResponse) { return true; }
bool AConceptCharacter::SetCapsuleCollisionResponseOnMulticast_Validate(UPrimitiveComponent* Primitive, ECollisionChannel Channel, ECollisionResponse NewResponse) { return true; }
bool AConceptCharacter::MulticastSetSpeed_Validate(float SpeedParam) { return true; }
bool AConceptCharacter::ServerSetSpeed_Validate(float SpeedParam) { return true; }
bool AConceptCharacter::SetOrientationToMovement_Multicast_Validate(bool OrientedToMovement) { return true; }
bool AConceptCharacter::SetOrientationToMovement_Server_Validate(bool OrientedToMovement) { return true; }
bool AConceptCharacter::SetShield_Validate(float ShieldParam) { return true; }
bool AConceptCharacter::EnableShieldServer_Validate() { return true; }
bool AConceptCharacter::DisableShieldServer_Validate() { return true; }
bool AConceptCharacter::ServerSetLife_Validate(float LifeParam){return true;}
bool AConceptCharacter::ServerSetLifeMax_Validate(float LifeParam){return true;}
bool AConceptCharacter::MulticastSetLifeReg_Validate(float LifeParam){return true;}
bool AConceptCharacter::ServerSetLifeReg_Validate(float LifeParam){return true;}
bool AConceptCharacter::MulticastSetJump_Validate(float JumpParam){return true;}
bool AConceptCharacter::ServerSetJump_Validate(float JumpParam){return true;}
bool AConceptCharacter::SetCanReceiveDamage_Validate(bool CanReceive){return true;}
bool AConceptCharacter::CanShoot_Validate(bool bCanShoot){return true;}
bool AConceptCharacter::ReplicateGravity_Validate(bool bActivateZeroGravity, float Direction){return true;}
bool AConceptCharacter::StartDisruptStatusServer_Validate(float DisruptTime) {return true;}
bool AConceptCharacter::MulticastSetSprintSpeed_Validate() { return true; }
bool AConceptCharacter::ServerSetSprintSpeed_Validate() { return true; }
bool AConceptCharacter::MulticastSetNormalSpeed_Validate() { return true; }
bool AConceptCharacter::ServerSetNormalSpeed_Validate() { return true; }
bool AConceptCharacter::Server_MakeInvisible_Validate(UMaterialInterface* NewMat, bool Visible = true) { return true; }
bool AConceptCharacter::PlaySound_Server_Validate(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol) {return true;}
bool AConceptCharacter::StopSound_Server_Validate(UAudioComponent* Component) {return true;}
bool AConceptCharacter::PlaySound_Client_Validate(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol) { return true; }
bool AConceptCharacter::StopSound_Client_Validate(UAudioComponent* Component) { return true; }
bool AConceptCharacter::PlaySound_Multicast_Validate(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol) {return true;}
bool AConceptCharacter::StopSound_Multicast_Validate(UAudioComponent* Component) {return true;}
bool AConceptCharacter::SetAbilityVisibility_Validate(bool bIsVisible, AAbility* Ability) { return true; }
